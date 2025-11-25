//
//  GeneratorEcsCpp.cpp
//  mlc-cpp
//
//  C++ port of plugin/ecs_generate.py logic
//

#include "../features/GeneratorEcsCpp.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <re2/re2.h>
#include <sstream>

#include "../core/Parser.hpp"
#include "../models/Class.hpp"
#include "../models/Function.hpp"
#include "../models/Model.hpp"
#include "../models/Object.hpp"
#include "../utils/Common.hpp"

using std::shared_ptr;
using std::string;

// --- helpers ---
std::string MODEL_ECS_TEMPLATES = R"__EACH__(
public:
    template <class TComp>
    void remove_if(const std::function<bool(intrusive_ptr<TComp>&)>& cond)
    {
        auto& components = this->get_components<TComp>();
        auto& map_components = this->get_map_components<TComp>();
        auto iter = std::remove_if(components.begin(), components.end(), [&](auto& comp)
            {
                if(cond(comp))
                {
                    map_remove(map_components, comp->id);
                    return true;
                }
                return false;
            });
        if(iter != components.end())
        {
            components.erase(iter, components.end());
        }
    }
    
    template <class TCompA, class TCompB>
    std::vector<std::tuple<intrusive_ptr<TCompA>, intrusive_ptr<TCompB>>> view()const
    {
        auto& cont_a = get_components<TCompA>();
        auto& cont_b = get_components<TCompB>();
        assert(std::is_sorted(std::begin(cont_a), std::end(cont_a), [](auto const& lhs, auto const& rhs){return lhs->id < rhs->id;}));
        assert(std::is_sorted(std::begin(cont_b), std::end(cont_b), [](auto const& lhs, auto const& rhs){return lhs->id < rhs->id;}));
        
        std::vector<std::tuple<intrusive_ptr<TCompA>, intrusive_ptr<TCompB>>> result;
        result.reserve(std::min(cont_a.size(), cont_b.size()));
        
        size_t i = 0, j = 0;
        auto size_a = cont_a.size();
        auto size_b = cont_b.size();
        while (i < size_a && j < size_b)
        {
            auto& comp_a = cont_a[i];
            auto& comp_b = cont_b[j];
            int ida = comp_a->id;
            int idb = comp_b->id;
            if (ida == idb)
            {
                result.emplace_back(comp_a, comp_b);
                ++i; ++j;
            }
            else if (ida < idb)
            {
                ++i;
            }
            else
            {
                ++j;
            }
        }
        return result;
    }
    
    template<typename A, typename B, typename... Rest>
    auto view_all() const
    {
        return join_all(view<A, B>(), get_components<Rest>()...);
    }
    
    template<typename A, typename Func>
    void each(Func func)
    {
        auto& components = get_components<A>();
        for(auto& comp : components)
        {
            func(comp);
        }
    }

    template<typename A, typename B, typename... Rest, typename Func>
    void each(Func func)
    {
        // запускаем внутренний имплементационный метод с индексовой последовательностью
        each_impl<A,B,Rest...>(std::index_sequence_for<A,B,Rest...>{},func);
    }
    
    template<typename A, typename B, typename... Rest, typename Cond, typename Func>
    void each_if(Cond cond, Func func)
    {
        // запускаем внутренний имплементационный метод с индексовой последовательностью
        each_if_impl<A,B,Rest...>(std::index_sequence_for<A,B,Rest...>{},func, cond);
    }
    
private:
    //  Имплементация N-way merge через tuple<references> + индексы
    template<typename A, typename B, typename... Rest, size_t... Is, typename Func>
    void each_impl(std::index_sequence<Is...>, Func func)
    {
        // 1) Собираем tuple из ссылок на наши контейнеры
        auto containers = std::tie(get_components<A>(),
                                   get_components<B>(),
                                   get_components<Rest>()...);
        constexpr size_t N = 2 + sizeof...(Rest);
        
        // 2) Индексы по каждому вектору и их размеры
        std::array<size_t, N> idxs{};
        std::array<size_t, N> sizes
        {
            std::get<Is>(containers).size()...
        };
        
        // 3) N-way merge
        while (true)
        {
            // если любой индекс вышел за пределы — закончили
            if (((idxs[Is] >= sizes[Is]) || ...)) break;
            
            // читаем все текущие id
            std::array<int, N> ids
            {
                std::get<Is>(containers)[idxs[Is]]->id...
            };
            
            // найдём максимальный id
            int max_id = ids[0];
            for (size_t k = 1; k < N; ++k)
            {
                if (ids[k] > max_id) max_id = ids[k];
            }
            
            // если все id совпадают — вызовем func и сдвинем все индексы
            if (((ids[Is] == max_id) && ...))
            {
                func(std::get<Is>(containers)[idxs[Is]]...);
                (++idxs[Is], ...);
            }
            else
            {
                // иначе продвигаем лишь те указатели, у которых id < max_id
                for (size_t k = 0; k < N; ++k)
                {
                    if (ids[k] < max_id)
                    {
                        ++idxs[k];
                    }
                }
            }
        }
    }
    //  Имплементация N-way merge через tuple<references> + индексы
    template<typename A, typename B, typename... Rest, size_t... Is, typename Func, typename Cond>
    void each_if_impl(std::index_sequence<Is...>, Func func, Cond cond)
    {
        // 1) Собираем tuple из ссылок на наши контейнеры
        auto containers = std::tie(get_components<A>(),
                                   get_components<B>(),
                                   get_components<Rest>()...);
        constexpr size_t N = 2 + sizeof...(Rest);
        
        // 2) Индексы по каждому вектору и их размеры
        std::array<size_t, N> idxs{};
        std::array<size_t, N> sizes
        {
            std::get<Is>(containers).size()...
        };
        // 3) N-way merge
        while (true)
        {
            // если любой индекс вышел за пределы — закончили
            if (((idxs[Is] >= sizes[Is]) || ...)) break;
            
            // читаем все текущие id
            std::array<int, N> ids
            {
                std::get<Is>(containers)[idxs[Is]]->id...
            };
            
            // найдём максимальный id
            int max_id = ids[0];
            for (size_t k = 1; k < N; ++k)
            {
                if (ids[k] > max_id) max_id = ids[k];
            }
            
            // если все id совпадают — вызовем func и сдвинем все индексы
            if (((ids[Is] == max_id) && ...))
            {
                if(cond(std::get<Is>(containers)[idxs[Is]]...))
                    func(std::get<Is>(containers)[idxs[Is]]...);
                (++idxs[Is], ...);
            }
            else
            {
                // иначе продвигаем лишь те указатели, у которых id < max_id
                for (size_t k = 0; k < N; ++k)
                {
                    if (ids[k] < max_id)
                    {
                        ++idxs[k];
                    }
                }
            }
        }
    }
)__EACH__";

static string to_snake(const string &s) {
    string out;
    out.reserve(s.size() * 2);
    for (size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];
        if (std::isupper(static_cast<unsigned char>(ch)) && i > 0)
            out.push_back('_');
        out.push_back(
            static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return out;
}

bool GeneratorEcsCpp::isBased(const shared_ptr<Class> &cls,
                              const string &name) {
    if (!cls)
        return false;
    if (cls->name == "Movement2")
        std::cout << "";
    if (cls->name == name)
        return true;
    if (cls->parent.expired())
        cls->parent = _model->get_class(cls->parent_class_name);
    if (!cls->parent.expired()) {
        auto p = cls->parent.lock();
        if (isBased(p, name))
            return true;
    } else if (!cls->parent_class_name.empty()) {
        // parent will be linked later, but we can compare by name if same group
        // assumed
        if (cls->parent_class_name == name)
            return true;
    }
    return false;
}

string GeneratorEcsCpp::componentsField(const shared_ptr<Class> &cls) {
    // ComponentFoo -> foo, CamelCase to snake
    string n = cls->name;
    const string pref = "Component";
    if (n.rfind(pref, 0) == 0)
        n = n.substr(pref.size());
    return to_snake(n);
}

shared_ptr<Class> GeneratorEcsCpp::getClass(Model &model, const string &name) {
    return model.get_class(name);
}

Function GeneratorEcsCpp::makeFnDecl(const string &decl) {
    return parse_function(decl);
}

Object GeneratorEcsCpp::makeObj(const string &decl, bool withName) {
    return parse_object(decl, withName);
}

GeneratorEcsCpp::GeneratorEcsCpp(const FeatureEcs &feature) {
    _ecs_model_base_name = feature.model_base;
    _ecs_component_base_name = feature.component_base;
    if (_ecs_model_base_name.empty())
        _ecs_model_base_name = "ModelEcsBase";
    if (_ecs_component_base_name.empty())
        _ecs_component_base_name = "ComponentBase";
}
// --- main entry ---
void GeneratorEcsCpp::generate(Model &model) {
    _model = &model;
    if(model.config.language != "cpp"){
        return;
    }
    if (!model.hasClass(_ecs_model_base_name))
        return;
    auto ecsBase = getClass(model, _ecs_model_base_name);
    if (!ecsBase){
        std::cout << "Cannot find ECS model base class: " << _ecs_model_base_name << std::endl;
        return;
    }
    
    createPimplClass(model, ecsBase);
    createPimplMember(model, ecsBase);
    
    auto ecsPimpl = getClass(model, "EcsPimplImpl");
    
    generateContainers(model, ecsPimpl);
    generateClearComponents(model, ecsBase);

    generateAddModelMethod(model);
    generateRemoveModelMethod(model);
    generateGetSelfFromModelMethod(model);
    generateHasInModel(model);

    generateModelAddComponent(model);
    generateModelRemoveComponent(model, /*useRawPointer*/ true);
    generateModelRemoveComponent(model, /*useRawPointer*/ false);
    generateModelGetComponent(model, /*isConst*/ true);
    generateModelGetComponent(model, /*isConst*/ false);
    generateModelCopyEntityFromModel(model);
    generateModelGetComponents(model, /*isConst*/ true);
    generateModelGetComponents(model, /*isConst*/ false);
    generateModelGetMapComponents(model, false);
    generateModelGetMapComponents(model, true);
    
    generateSystemsEndTurn(model, ecsBase);
    for(auto cls : model.classes){
        if(cls->parent_class_name == _ecs_model_base_name){
            generateSystemsEndTurn(model, cls);
        }
    }
    generateBuildMaps(model);
    generateRemoveEntity(model);
    

    generate_system_skills(model, "update");
    generate_system_skills(model, "clean");
    generate_model_method_save_skills(model);

    // provide helper header file (optional utilities)
    addHelperFile(model);
}

std::vector<shared_ptr<Class>>
GeneratorEcsCpp::get_skill_components(Model &model) {
    std::vector<shared_ptr<Class>> classes;
    for (auto &cls : model.classes) {
        if (isBased(cls, "ComponentSkillBase") &&
            cls->name != "ComponentSkillBase")
            classes.push_back(cls);
    }
    return classes;
}

void GeneratorEcsCpp::generate_system_skills(Model &model,
                                             const std::string &method_name) {
    auto skills = get_skill_components(model);
    auto system = getClass(model, "SystemSkillBase");
    if (!system)
        return;
    Function *method = nullptr;
    for (auto &f : system->functions) {
        if (f.name == method_name) {
            method = &f;
            break;
        }
    }
    if (!method)
        return;
    std::string operations = method->body;
    method->body.clear();
    for (auto &skill : skills) {
        if (!skill->subclasses.empty())
            continue;
        auto field = componentsField(skill);
        std::string list_name = std::string("components_") + field;
        std::string block = operations;
        replace_all(block, "@(components)", list_name);
        if (!method->body.empty())
            method->body += "\n";
        method->body += block;
        system->user_includes.insert(skill->name);
    }
}

void GeneratorEcsCpp::generate_model_method_save_skills(Model &model) {
    auto controller = getClass(model, "ControllerDungeonBase");
    if (!controller)
        return;
    Function *save_skills_current_hero = nullptr;
    Function *restore_hero_skill_on_change = nullptr;
    for (auto &f : controller->functions) {
        if (f.name == "save_skills_current_hero")
            save_skills_current_hero = &f;
        if (f.name == "restore_hero_skill_on_change")
            restore_hero_skill_on_change = &f;
        if (restore_hero_skill_on_change && save_skills_current_hero)
            break;
    }
    if (!save_skills_current_hero)
        return;
    if (!restore_hero_skill_on_change)
        return;

    auto info = getClass(model, "ModelDungeonBaseChangeHeroesInfo");
    if (!info)
        return;

    if (!save_skills_current_hero->body.empty())
        save_skills_current_hero->body += "\n";
    save_skills_current_hero->body += "auto id = this->model->player_id;\n";
    save_skills_current_hero->body +=
        "auto name = this->model->get<ComponentData>(id)->data->name;";

    if (!restore_hero_skill_on_change->body.empty())
        restore_hero_skill_on_change->body += "\n";
    restore_hero_skill_on_change->body +=
        "auto name = "
        "this->model->get<ComponentData>(this->model->player_id)->data->name;"
        "\n";

    auto skills = get_skill_components(model);
    for (auto &skill : skills) {
        auto field = componentsField(skill);
        {
            std::string line = format_indexes(
                R"(this->model->change_heroes_info.{0}[name] = this->model->get<{1}>(id);)",
                field, skill->name);
            if (!save_skills_current_hero->body.empty())
                save_skills_current_hero->body += "\n";
            save_skills_current_hero->body += line;
        }
        {
            std::string a = format_indexes(
                R"(if(in_map(name, this->model->change_heroes_info.{0}) && this->model->change_heroes_info.{0}[name]))",
                field);
            if (!restore_hero_skill_on_change->body.empty())
                restore_hero_skill_on_change->body += "\n";
            restore_hero_skill_on_change->body += a;
            restore_hero_skill_on_change->body += "\n{";
            std::string b = format_indexes(
                R"(    this->model->add<{0}>(this->model->change_heroes_info.{1}[name]);)",
                skill->name, field);
            restore_hero_skill_on_change->body += "\n";
            restore_hero_skill_on_change->body += b;
            restore_hero_skill_on_change->body += "\n}";
        }
        {
            std::string member =
                format_indexes(R"(map<string, {0}*> {1})", skill->name, field);
            info->members.push_back(makeObj(member, true));
        }
    }
}

void GeneratorEcsCpp::modifySources(Model &model,
                                    const std::shared_ptr<Class> &cls,
                                    std::string &header, std::string &source) {
    if (cls->name != _ecs_model_base_name)
        return;

    replace_all(header, "};", MODEL_ECS_TEMPLATES + "\n};");
}

void GeneratorEcsCpp::createPimplClass(Model &model, const std::shared_ptr<Class> &ecsBase){
    auto pimpl = std::make_shared<Class>();
    pimpl->name = "EcsPimpl";
    pimpl->type = "class";
    pimpl->group = ecsBase->group;
    model.add_class(pimpl);
    
    auto impl = std::make_shared<Class>();
    impl->name = "EcsPimplImpl";
    impl->type = "class";
    impl->group = ecsBase->group;
    impl->parent_class_name = "EcsPimpl";
    impl->parent = pimpl;
    model.add_class(impl);
    
    pimpl->subclasses.push_back(impl);
}

void GeneratorEcsCpp::createPimplMember(Model &model, const std::shared_ptr<Class> &ecsBase){
    ecsBase->members.push_back(parse_object("EcsPimpl:private:pointer _pimpl", true));
    ecsBase->user_includes.insert("EcsPimplImpl");
    
    Function ctr;
    ctr.name = _ecs_model_base_name;
    ctr.body = "\n_pimpl = new EcsPimplImpl();\n";
    ecsBase->constructors.push_back(std::move(ctr));
}

void GeneratorEcsCpp::generateContainers(
    Model &model, const shared_ptr<Class> &ecsBase) {
    for (auto &cls : model.classes) {
        if (cls->name == "Transform")
            std::cout << "";
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        // list<Cls*> components_field
        {
            std::string decl = format_indexes(R"(list<{0}*> components_{1})",
                                              cls->name, field);
            ecsBase->members.push_back(makeObj(decl, true));
        }
        // map<int, Cls*>:private:runtime map_components_field
        {
            std::string decl = format_indexes(
                R"(map<int, {0}*>:runtime map_components_{1})",
                cls->name, field);
            ecsBase->members.push_back(makeObj(decl, true));
        }
    }
}


void GeneratorEcsCpp::generateClearComponents(Model &model, const std::shared_ptr<Class> &ecsBase){

    auto fn = makeFnDecl("fn<T> void clear()");
    
    static std::string BODY = R"(
template<> void {0}::clear<{1}>()
{
    list_clear(static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->components_{2});
    map_clear(static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{2});
})";
    
    for (auto &cls : getComponentClasses(model)) {
        if (!isBased(cls, _ecs_component_base_name) || cls->name == _ecs_component_base_name)
            continue;

        auto field = componentsField(cls);
        std::string body;
        body += format_indexes(BODY, _ecs_model_base_name, cls->name, field);
        fn.specific_implementations += body;
    }
    fn.specific_implementations += "\n\n";
    ecsBase->functions.push_back(std::move(fn));
}

void GeneratorEcsCpp::generateSystemsEndTurn(Model &model, const std::shared_ptr<Class> &ecs) {
    if (!ecs)
        return;
    
    // move method to end (all templates in begin class declaration)
    {
        auto update = *ecs->get_method("update_systems");
        auto iter = std::find_if(ecs->functions.begin(), ecs->functions.end(), [](auto& f){return f.name == "update_systems";});
        ecs->functions.erase(iter);
        ecs->functions.push_back(std::move(update));
    }
    
    Function *update = ecs->get_method("update_systems");;
    if (!update)
        return;

    // parse body by lines and expand directives like [SystemX] and optional
    // condition [!isFoo]
    std::vector<string> lines;
    {
        std::istringstream iss(update->body);
        string line;
        while (std::getline(iss, line))
            lines.push_back(line);
    }
    std::vector<string> out;
    static re2::RE2 reCond(R"(\[([!]*is\w+)\])");
    static re2::RE2 reSys(R"(\[(System\w+)\])");
    for (size_t i = 0; i < lines.size(); ++i) {
        const auto &op = lines[i];
        if (op.empty())
            continue;
        std::string sys;
        bool hasS = RE2::PartialMatch(op, reSys, &sys);
        if (hasS) {
            string cond;
            (void)RE2::PartialMatch(op, reCond, &cond);
            // check if SystemX has method clean
            bool hasClean = false;
            auto sysCls = getClass(model, sys);
            if (sysCls) {
                for (auto &fn : sysCls->functions) {
                    if (fn.name == "clean") {
                        hasClean = true;
                        break;
                    }
                }
            }
            std::string args; // this, dt
            auto update = sysCls->get_method("update");
            if (!update)
                continue;
            for (auto &arg : update->callable_args) {
                if (!args.empty())
                    args += ", ";
                if (arg.type == _ecs_model_base_name)
                    args += "this";
                else if (arg.type == "float" && arg.name == "dt")
                    args += "dt";
                else
                    assert(0);
            }
            std::string code;
            if (!cond.empty())
                code += format_indexes(R"(if({0})
{
)",
                                       cond);
            code += format_indexes(R"({0} s{1};
)",
                                   sys, std::to_string(i));
            code +=
                format_indexes(R"(s{0}.update({1});)", std::to_string(i), args);
            if (hasClean)
                code += format_indexes(R"(
s{0}.clean(this);)",
                                       std::to_string(i));
            if (!cond.empty())
                code += "\n}";
            out.push_back(code);
        } else {
            // keep as-is, ensure it ends with ';' if not brace
            string trimmed = op;
            auto p = trimmed.find_last_not_of(" \t\r\n");
            if (p != string::npos)
                trimmed.resize(p + 1);
            if (!trimmed.empty() && trimmed.back() != ';' &&
                trimmed.back() != '{' && trimmed.back() != '}')
                trimmed += ';';
            out.push_back(trimmed);
        }
    }
    std::string joined;
    for (size_t i = 0; i < out.size(); ++i) {
        if (i)
            joined += "\n";
        joined += out[i];
    }
    update->body = joined;
}

void GeneratorEcsCpp::generateAddModelMethod(Model &model) {
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name))
            continue;
        auto fn = makeFnDecl("fn void add_self_to_model(" +
                             _ecs_model_base_name + "* model_dungeon_class)");
        if (cls->name != _ecs_component_base_name) {
            fn.body = format_indexes(R"(model_dungeon_class->add<{0}>(this);)",
                                     cls->name);
        } else {
            fn.is_abstract = true;
        }
        fn.is_virtual = true;
        cls->functions.push_back(std::move(fn));
    }
}

void GeneratorEcsCpp::generateRemoveModelMethod(Model &model) {
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name))
            continue;
        auto fn = makeFnDecl("fn void remove_self_from_model(" +
                             _ecs_model_base_name + "* model_dungeon_class)");
        if (cls->name != _ecs_component_base_name) {
            fn.body = format_indexes(
                R"(model_dungeon_class->remove<{0}>(this);)", cls->name);
        } else {
            fn.is_abstract = true;
        }
        fn.is_virtual = true;
        cls->functions.push_back(std::move(fn));
    }
}

void GeneratorEcsCpp::generateGetSelfFromModelMethod(Model &model) {
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name))
            continue;
        if (cls->name == _ecs_component_base_name)
            continue;
        auto fn = makeFnDecl("fn " + _ecs_component_base_name +
                             "* get_self_from_model(" + _ecs_model_base_name +
                             "* model_dungeon_class, int id)");
        fn.body = format_indexes(R"(return model_dungeon_class->get<{0}>(id);)",
                                 cls->name);
        fn.is_virtual = true;
        cls->functions.push_back(std::move(fn));
    }
}

void GeneratorEcsCpp::generateHasInModel(Model &model) {
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentSkillBase"))
            continue;
        if (cls->name == "ComponentSkillBase")
            continue;
        auto fn = makeFnDecl("fn bool has_in_model(" + _ecs_model_base_name +
                             "* model_dungeon_class, int id)");
        fn.body = format_indexes(
            R"(return model_dungeon_class->get<{0}>(id) != nullptr;)",
            cls->name);
        fn.is_virtual = true;
        cls->functions.push_back(std::move(fn));
    }
}

void GeneratorEcsCpp::generateBuildMaps(Model &model) {
    auto ecs = getClass(model, _ecs_model_base_name);
    if (!ecs)
        return;
    auto fn = makeFnDecl("fn void build_maps()");
    std::string body;
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        body += format_indexes(R"(for(auto& component : static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->components_{0})
{
    static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{0}[component->id] = component;
}
)",
                               field);
    }
    fn.body = body;
    ecs->functions.push_back(std::move(fn));
}

void GeneratorEcsCpp::generateRemoveEntity(Model &model) {
    auto ecs = getClass(model, _ecs_model_base_name);
    if (!ecs)
        return;
    Function *fn = nullptr;
    for (auto &f : ecs->functions)
        if (f.name == "remove_entity") {
            fn = &f;
            break;
        }
    if (!fn)
        return;
    std::string add;
    if (!fn->body.empty())
        add += fn->body + "\n";
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        add += format_indexes(R"(if(in_map(id, static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{0}))
{
    auto component = static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{0}.at(id);
    this->remove<{1}>(component);
}
)",
                              field, cls->name);
    }
    fn->body = add;
}

void GeneratorEcsCpp::generateModelGetComponent(Model &model, bool isConst) {
    auto ecs = getClass(model, _ecs_model_base_name);
    if (!ecs)
        return;
    Function method;
    method.name = "get";
    // template <class T>
    Object tpl;
    tpl.type = "T";
    method.template_args.push_back(tpl);
    // return type
    if (isConst) {
        method.return_type.type = "T";
        method.return_type.is_pointer = true;
        method.return_type.is_const = true;
    } else {
        method.return_type.type = "intrusive_ptr";
        Object inner;
        inner.type = "T";
        method.return_type.template_args.push_back(inner);
    }
    // args
    method.callable_args.push_back(makeObj("int component_id", true));
    method.is_const = isConst;
    ecs->functions.insert(ecs->functions.begin(), method);

    // specializations
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        std::string retType = isConst ? ("const " + cls->name + "*")
                                      : ("intrusive_ptr<" + cls->name + ">");
        std::string spec;
        spec += format_indexes(R"(template<> {0} {1}::get(int component_id){2}
{
    if(in_map(component_id, static_cast<{2} EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{3}))
    {
        return static_cast<{2} EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{3}.at(component_id);
    }
    return nullptr;
}

)",
                               retType, _ecs_model_base_name,
                               (isConst ? " const" : ""), field);
        ecs->functions.front().specific_implementations += spec;
    }
}

std::vector<shared_ptr<Class>>
GeneratorEcsCpp::getComponentClasses(Model &model) {
    std::vector<shared_ptr<Class>> out;
    for (auto &cls : model.classes) {
        if (isBased(cls, _ecs_component_base_name) &&
            cls->name != _ecs_component_base_name) {
            if (cls->name == "ComponentSkillBase")
                out.insert(out.begin(), cls);
            else
                out.push_back(cls);
        }
    }
    return out;
}

void GeneratorEcsCpp::generateModelAddComponent(Model &model) {
    auto ecs = getClass(model, _ecs_model_base_name);
    if (!ecs)
        return;

    // template <class T> void add(intrusive_ptr<T> component, int
    // component_id=0)
    {
        Function m;
        m.name = "add";
        Object tpl;
        tpl.type = "T";
        m.template_args.push_back(tpl);
        m.return_type.type = "void";
        // args
        Object arg0;
        arg0.type = "intrusive_ptr";
        {
            Object inner;
            inner.type = "T";
            arg0.template_args.push_back(inner);
        }
        arg0.name = "component";
        m.callable_args.push_back(arg0);
        Object arg1;
        arg1.type = "int";
        arg1.name = "component_id";
        arg1.value = "0";
        m.callable_args.push_back(arg1);
        ecs->functions.push_back(m);
        auto &decl1 = ecs->functions.back();

        for (auto &cls : getComponentClasses(model)) {
            auto field = componentsField(cls);
            std::string body;
            body += format_indexes(
                R"(template<> void {0}::add(intrusive_ptr<{1}> component, int component_id)
{
)",
                _ecs_model_base_name, cls->name);
            if (!cls->parent_class_name.empty() &&
                cls->parent_class_name != _ecs_component_base_name) {
                body += format_indexes(
                    R"(    this->add<{0}>(intrusive_ptr<{0}>(component), component_id);
)",
                    cls->parent_class_name);
            }
            body +=
                R"(    assert(component->id == 0 || component->id == component_id || component_id == 0);
    if(component_id != 0)
{
component->id = component_id;
}
    assert(component->id > 0);
)";
            body += format_indexes(
                R"(    if(in_map(component->id, static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{0}))
{
list_remove(static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->components_{0}, static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{0}.at(component->id));
}
)",
                field);
            body += format_indexes(
R"(    auto iter = std::lower_bound(static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->components_{0}.begin(), static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->components_{0}.end(), component, [](const auto& a, const auto& b)
{
return a->id < b->id;
});
    static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->components_{0}.insert(iter, component);
    assert(std::is_sorted(static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->components_{0}.begin(), static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->components_{0}.end(), [](const auto& l, const auto& r)
{
return l->id < r->id;
}));
    static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{0}[component->id] = component;
}

)",
                field);
            decl1.specific_implementations += body;
        }
    }

    // template <class T> void add(T* component, int component_id=0) -> forward
    // to intrusive_ptr
    {
        Function m;
        m.name = "add";
        Object tpl;
        tpl.type = "T";
        m.template_args.push_back(tpl);
        m.return_type.type = "void";
        
        Object arg0;
        arg0.type = "T";
        arg0.is_pointer = true;
        arg0.name = "component";
        m.callable_args.push_back(arg0);
        
        Object arg1;
        arg1.type = "int";
        arg1.name = "component_id";
        arg1.value = "0";
        m.callable_args.push_back(arg1);
        ecs->functions.push_back(m);
        auto &decl2 = ecs->functions.back();
        for (auto &cls : model.classes) {
            if (!isBased(cls, _ecs_component_base_name) ||
                cls->name == _ecs_component_base_name)
                continue;
            auto field = componentsField(cls);
            (void)field;
            std::string body = format_indexes(
                R"(template<> void {0}::add({1}* component, int component_id)
{
this->add(intrusive_ptr<{1}>(component), component_id);
}
)",
                _ecs_model_base_name, cls->name);
            decl2.specific_implementations += body;
        }
    }
}

void GeneratorEcsCpp::generateModelRemoveComponent(Model &model,
                                                   bool useRawPointer) {
    auto ecs = getClass(model, _ecs_model_base_name);
    if (!ecs)
        return;
    Function m;
    m.name = "remove";
    Object tpl;
    tpl.type = "T";
    m.template_args.push_back(tpl);
    m.return_type.type = "void";
    if (useRawPointer) {
        Object arg;
        arg.type = "intrusive_ptr";
        {
            Object inner;
            inner.type = "T";
            arg.template_args.push_back(inner);
        }
        arg.name = "component";
        m.callable_args.push_back(arg);
    } else {
        Object arg;
        arg.type = "T";
        arg.is_pointer = true;
        arg.name = "component";
        m.callable_args.push_back(arg);
    }
    ecs->functions.insert(ecs->functions.begin(), m);
    auto &decl = ecs->functions.front();

    for (auto &cls : getComponentClasses(model)) {
        auto field = componentsField(cls);
        std::string body;
        if (useRawPointer) {
            body += format_indexes(
                R"(template<> void {0}::remove({1}* component)
{
this->remove(intrusive_ptr<{1}>(component));
}
)",
                _ecs_model_base_name, cls->name);
        } else {
            body += format_indexes(
                R"(template<> void {0}::remove(intrusive_ptr<{1}> component)
{
)",
                _ecs_model_base_name, cls->name);
            if (!cls->parent_class_name.empty() &&
                cls->parent_class_name != _ecs_component_base_name) {
                body += format_indexes(
                    R"(    this->remove<{0}>(intrusive_ptr<{0}>(component));
)",
                    cls->parent_class_name);
            }
            body += format_indexes(
                R"(    if(component)
{
list_remove(static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->components_{0}, component); map_remove(static_cast<EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{0}, component->id);
}
}

)",
                field);
        }
        decl.specific_implementations += body;
    }
}

void GeneratorEcsCpp::generateModelCopyEntityFromModel(Model &model) {
    auto ecs = getClass(model, _ecs_model_base_name);
    if (!ecs)
        return;
    auto m = makeFnDecl("fn void copy_entity_from_model(" +
                        _ecs_model_base_name + "* model, int id, int new_id)");
    std::string body;
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        body += format_indexes(R"(auto component_{0} = model->get<{1}>(id);
)",
                               field, cls->name);
        body += format_indexes(R"(if(component_{0})
{
)",
                               field);
        body +=
            format_indexes(R"(    auto clone = clone_object<{0}>(component_{1});
)",
                           cls->name, field);
        body += "    clone->id = new_id;\n";
        body += format_indexes(R"(    this->add<{0}>(clone, new_id);
)",
                               cls->name);
        body += "}\n";
    }
    m.body = body;
    ecs->functions.push_back(std::move(m));
}

void GeneratorEcsCpp::generateModelGetComponents(Model &model, bool isConst) {
    auto ecs = getClass(model, _ecs_model_base_name);

    Function m;
    m.name = "get_components";
    Object tpl;
    tpl.type = "T";
    m.template_args.push_back(tpl);
    m.return_type.type = "list"; // converts to std::vector
    m.return_type.is_ref = true;
    m.return_type.is_const = isConst;
    m.is_const = isConst;
    {
        Object intr;
        intr.type = "intrusive_ptr";
        Object inner;
        inner.type = "T";
        intr.template_args.push_back(inner);
        m.return_type.template_args.push_back(intr);
    }
    ecs->functions.insert(ecs->functions.begin(), m);
    auto &decl = ecs->functions.front();
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        std::string header = format_indexes(
            R"(template <> {0}std::vector<intrusive_ptr<{1}>>& {2}::get_components() {3}
)",
            (isConst ? std::string("const ") : std::string("")), cls->name,
            _ecs_model_base_name,
            (isConst ? std::string("const") : std::string("")));
        std::string body = format_indexes(
            R"({
return static_cast<{0}EcsPimplImpl*>(this->_pimpl.ptr())->components_{1}; 
}
)", isConst ? std::string("const ") : std::string(""), field);
        decl.specific_implementations += header + body;
    }
}

const std::string GET_MAP_COMPONENETS = R"(
    template <>{3} std::map<int, intrusive_ptr<{1}>>& {0}::get_map_components(){3}
    {
        return static_cast<{3} EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{2};
    }
)";

void GeneratorEcsCpp::generateModelGetMapComponents(Model &model, bool isConst) {
    auto ecs = getClass(model, _ecs_model_base_name);

    auto m = parse_function(format_indexes("fn<T> map<int, T*>:ref{0} get_map_components():protected{0}", isConst ? ":const" : ""));
    ecs->functions.insert(ecs->functions.begin(), std::move(m));
    auto method = ecs->get_method("get_map_components");
    assert(method);
    
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        std::string body = format_indexes(GET_MAP_COMPONENETS, _ecs_model_base_name, cls->name, field, isConst ? " const" : "");
        method->specific_implementations += body;
    }
}


void GeneratorEcsCpp::addHelperFile(Model &model) {
    static const char *HELPER_HPP = R"__(#ifndef __mg_ecs_helper_h__
#define __mg_ecs_helper_h__

#include <vector>
#include <tuple>
#include "intrusive_ptr.h"
#include <algorithm>

template<typename A, typename B>
auto join_two_sorted(
    const std::vector<intrusive_ptr<A>>& va,
    const std::vector<intrusive_ptr<B>>& vb)
{
    std::vector<std::pair<intrusive_ptr<A>, intrusive_ptr<B>>> result;
    result.reserve(std::min(va.size(), vb.size()));

    size_t ia = 0, ib = 0;
    while (ia < va.size() && ib < vb.size()) {
        int idA = va[ia]->id;
        int idB = vb[ib]->id;
        if      (idA < idB) ++ia;
        else if (idB < idA) ++ib;
        else {
            result.emplace_back(va[ia], vb[ib]);
            ++ia; ++ib;
        }
    }
    return result;
}

template<typename Tuple, typename C>
auto join_tuple_vector(
    const std::vector<Tuple>& partial,
    const std::vector<intrusive_ptr<C>>& vc)
{
    using new_tuple_t = decltype(
        std::tuple_cat(
            std::declval<Tuple>(),
            std::declval<std::tuple<intrusive_ptr<C>>>()
        )
    );

    std::vector<new_tuple_t> result;
    result.reserve(std::min(partial.size(), vc.size()));

    size_t i = 0, j = 0;
    while (i < partial.size() && j < vc.size()) {
        int idP = std::get<0>(partial[i])->id;
        int idC = vc[j]->id;
        if      (idP < idC) ++i;
        else if (idC < idP) ++j;
        else {
            result.emplace_back(
                std::tuple_cat(partial[i], std::make_tuple(vc[j]))
            );
            ++i; ++j;
        }
    }
    return result;
}

template<typename A, typename B>
auto join_all(
    const std::vector<intrusive_ptr<A>>& va,
    const std::vector<intrusive_ptr<B>>& vb)
{
    return join_two_sorted(va, vb);
}

template<typename Tuple, typename C, typename... Rest>
auto join_all(
    const std::vector<Tuple>&                  partial,
    const std::vector<intrusive_ptr<C>>&     vc,
    const std::vector<intrusive_ptr<Rest>>&... rest)
{
    auto next = join_tuple_vector(partial, vc);
    if constexpr(sizeof...(rest) > 0) {
        return join_all(next, rest...);
    }
    else {
        return next;
    }
}
#endif
)__";

    model.addFile(nullptr, "ecs/ecs_helper.h", HELPER_HPP);
}
