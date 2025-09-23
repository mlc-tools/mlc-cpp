//
//  GeneratorEcsCpp.cpp
//  mlc-cpp
//
//  C++ port of plugin/ecs_generate.py logic
//

#include "GeneratorEcsCpp.hpp"

#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>

#include "../models/Model.hpp"
#include "../models/Class.hpp"
#include "../models/Object.hpp"
#include "../models/Function.hpp"
#include "../core/Parser.hpp"
#include "../utils/Common.hpp"

using std::shared_ptr;
using std::string;

// --- helpers ---
std::string MODEL_ECS_TEMPLATES = R"__EACH__(
public:
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
    out.reserve(s.size()*2);
    for (size_t i=0;i<s.size();++i) {
        char ch = s[i];
        if (std::isupper(static_cast<unsigned char>(ch)) && i>0) out.push_back('_');
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    return out;
}

bool GeneratorEcsCpp::isBased(const shared_ptr<Class>& cls, const string &name) {
    if (!cls) return false;
    if (cls->group == "ecs") {
        if (cls->name == name) return true;
        if (!cls->parent.expired()) {
            auto p = cls->parent.lock();
            if (isBased(p, name)) return true;
        } else if (!cls->parent_class_name.empty()) {
            // parent will be linked later, but we can compare by name if same group assumed
            if (cls->parent_class_name == name) return true;
        }
    }
    return false;
}

string GeneratorEcsCpp::componentsField(const shared_ptr<Class>& cls) {
    // ComponentFoo -> foo, CamelCase to snake
    string n = cls->name;
    const string pref = "Component";
    if (n.rfind(pref, 0) == 0) n = n.substr(pref.size());
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

// --- main entry ---
void GeneratorEcsCpp::generate(Model &model) {
    if (!model.hasClass("ModelEcsBase")) return;
    auto ecsBase = getClass(model, "ModelEcsBase");
    if (!ecsBase) return;

    // members + clear_* methods per component
    generateContainersAndClear(model, ecsBase);

    // systems end-turn (update_systems body expansion)
    generateSystemsEndTurn(model);

    // base methods on ComponentBase hierarchy
    generateAddModelMethod(model);
    generateRemoveModelMethod(model);
    generateGetSelfFromModelMethod(model);
    generateHasInModel(model);

    // ModelEcsBase helpers
    generateBuildMaps(model);
    generateRemoveEntity(model);

    // API on ModelEcsBase
    generateModelAddComponent(model);
    generateModelRemoveComponent(model, /*useRawPointer*/true);
    generateModelRemoveComponent(model, /*useRawPointer*/false);
    generateModelGetComponent(model, /*isConst*/true);
    generateModelGetComponent(model, /*isConst*/false);
    generateModelCopyEntityFromModel(model);
    generateModelGetComponents(model, /*isConst*/true);
    generateModelGetComponents(model, /*isConst*/false);

    // provide helper header file (optional utilities)
    addHelperFile(model);
}

void GeneratorEcsCpp::modifySources(Model& model, const std::shared_ptr<Class>& cls, std::string& header, std::string& source){
    if(cls->name != "ModelEcsBase")
        return;
    
    replace_all(header, "};", MODEL_ECS_TEMPLATES + "\n};");
}

void GeneratorEcsCpp::generateContainersAndClear(Model &model, const shared_ptr<Class>& ecsBase) {
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentBase") || cls->name == "ComponentBase") continue;
        auto field = componentsField(cls);
        // list<Cls*> components_field
        {
            std::ostringstream ss;
            ss << "list<" << cls->name << "*> components_" << field;
            ecsBase->members.push_back(makeObj(ss.str(), true));
        }
        // map<int, Cls*>:private:runtime map_components_field
        {
            std::ostringstream ss;
            ss << "map<int, " << cls->name << "*>:private:runtime map_components_" << field;
            ecsBase->members.push_back(makeObj(ss.str(), true));
        }
        // fn void clear_components_field()
        {
            std::ostringstream decl;
            decl << "fn void clear_components_" << field << "()";
            auto fn = makeFnDecl(decl.str());
            std::ostringstream body;
            body << "map_clear(this->map_components_" << field << ");\n";
            body << "list_clear(this->components_" << field << ");";
            fn.body = body.str();
            ecsBase->functions.push_back(std::move(fn));
        }
    }
    // keep order: optional; C++ codegen doesn’t require sort here
}

void GeneratorEcsCpp::generateSystemsEndTurn(Model &model) {
    auto ecs = getClass(model, "ModelEcsBase");
    if (!ecs) return;
    Function *update = nullptr;
    for (auto &f : ecs->functions) if (f.name == "update_systems") { update = &f; break; }
    if (!update) return;

    // parse body by lines and expand directives like [SystemX] and optional condition [!isFoo]
    std::vector<string> lines;
    {
        std::istringstream iss(update->body);
        string line;
        while (std::getline(iss, line)) lines.push_back(line);
    }
    std::vector<string> out;
    std::regex reCond(R"(\[([!]*is\w+)\])");
    std::regex reSys (R"(\[(System\w+)\])");
    for (size_t i=0;i<lines.size();++i) {
        const auto &op = lines[i];
        if (op.empty()) continue;
        std::smatch mC, mS;
        bool hasS = std::regex_search(op, mS, reSys);
        if (hasS) {
            string cond;
            if (std::regex_search(op, mC, reCond)) cond = mC[1];
            string sys = mS[1];
            // check if SystemX has method clean
            bool hasClean = false;
            auto sysCls = getClass(model, sys);
            if (sysCls) {
                for (auto &fn : sysCls->functions) if (fn.name == "clean") { hasClean = true; break; }
            }
            std::ostringstream code;
            if (!cond.empty()) code << "if(" << cond << ")\n{\n";
            code << sys << " s" << i << ";\n";
            code << "s" << i << ".update(this, dt);";
            if (hasClean) code << "\n" << "s" << i << ".clean(this);";
            if (!cond.empty()) code << "\n}";
            out.push_back(code.str());
        } else {
            // keep as-is, ensure it ends with ';' if not brace
            string trimmed = op;
            auto p = trimmed.find_last_not_of(" \t\r\n");
            if (p!=string::npos) trimmed.resize(p+1);
            if (!trimmed.empty() && trimmed.back()!=';' && trimmed.back()!='{' && trimmed.back()!='}')
                trimmed += ';';
            out.push_back(trimmed);
        }
    }
    std::ostringstream oss;
    for (size_t i=0;i<out.size();++i) {
        if (i) oss << "\n";
        oss << out[i];
    }
    update->body = oss.str();
}

void GeneratorEcsCpp::generateAddModelMethod(Model &model) {
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentBase")) continue;
        auto fn = makeFnDecl("fn void add_self_to_model(ModelEcsBase* model_dungeon_class)");
        if (cls->name != "ComponentBase") {
            std::ostringstream body;
            body << "model_dungeon_class->add<" << cls->name << ">(this);";
            fn.body = body.str();
        } else {
            fn.is_abstract = true;
        }
        fn.is_virtual = true;
        cls->functions.push_back(std::move(fn));
    }
}

void GeneratorEcsCpp::generateRemoveModelMethod(Model &model) {
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentBase")) continue;
        auto fn = makeFnDecl("fn void remove_self_from_model(ModelEcsBase* model_dungeon_class)");
        if (cls->name != "ComponentBase") {
            std::ostringstream body;
            body << "model_dungeon_class->remove<" << cls->name << ">(this);";
            fn.body = body.str();
        } else {
            fn.is_abstract = true;
        }
        fn.is_virtual = true;
        cls->functions.push_back(std::move(fn));
    }
}

void GeneratorEcsCpp::generateGetSelfFromModelMethod(Model &model) {
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentBase")) continue;
        if (cls->name == "ComponentBase") continue;
        auto fn = makeFnDecl("fn ComponentBase* get_self_from_model(ModelEcsBase* model_dungeon_class, int id)");
        std::ostringstream body;
        body << "return model_dungeon_class->get<" << cls->name << ">(id);";
        fn.body = body.str();
        fn.is_virtual = true;
        cls->functions.push_back(std::move(fn));
    }
}

void GeneratorEcsCpp::generateHasInModel(Model &model) {
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentSkillBase")) continue;
        if (cls->name == "ComponentSkillBase") continue;
        auto fn = makeFnDecl("fn bool has_in_model(ModelEcsBase* model_dungeon_class, int id)");
        std::ostringstream body;
        body << "return model_dungeon_class->get<" << cls->name << ">(id) != nullptr;";
        fn.body = body.str();
        fn.is_virtual = true;
        cls->functions.push_back(std::move(fn));
    }
}

void GeneratorEcsCpp::generateBuildMaps(Model &model) {
    auto ecs = getClass(model, "ModelEcsBase");
    if (!ecs) return;
    auto fn = makeFnDecl("fn void build_maps()");
    std::ostringstream body;
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentBase") || cls->name == "ComponentBase") continue;
        auto field = componentsField(cls);
        body << "for(auto& component : this->components_" << field << ")\n";
        body << "{\n";
        body << "    this->map_components_" << field << "[component->id] = component;\n";
        body << "}\n";
    }
    fn.body = body.str();
    ecs->functions.push_back(std::move(fn));
}

void GeneratorEcsCpp::generateRemoveEntity(Model &model) {
    auto ecs = getClass(model, "ModelEcsBase");
    if (!ecs) return;
    Function *fn = nullptr;
    for (auto &f : ecs->functions) if (f.name == "remove_entity") { fn = &f; break; }
    if (!fn) return;
    std::ostringstream add;
    if (!fn->body.empty()) add << fn->body << "\n";
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentBase") || cls->name == "ComponentBase") continue;
        auto field = componentsField(cls);
        add << "if(in_map(id, this->map_components_" << field << "))\n";
        add << "{\n";
        add << "    auto component = this->map_components_" << field << ".at(id);\n";
        add << "    this->remove<" << cls->name << ">(component);\n";
        add << "}\n";
    }
    fn->body = add.str();
}

void GeneratorEcsCpp::generateModelGetComponent(Model &model, bool isConst) {
    auto ecs = getClass(model, "ModelEcsBase");
    if (!ecs) return;
    Function method;
    method.name = "get";
    // template <class T>
    Object tpl; tpl.type = "T"; method.template_args.push_back(tpl);
    // return type
    if (isConst) {
        method.return_type.type = "T";
        method.return_type.is_pointer = true;
        method.return_type.is_const = true;
    } else {
        method.return_type.type = "intrusive_ptr";
        Object inner; inner.type = "T"; method.return_type.template_args.push_back(inner);
    }
    // args
    method.callable_args.push_back(makeObj("int component_id", true));
    method.is_const = isConst;
    ecs->functions.insert(ecs->functions.begin(), method);

    // specializations
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentBase") || cls->name=="ComponentBase") continue;
        auto field = componentsField(cls);
        std::ostringstream body;
        body << "template<> "
             << (isConst ? ("const "+cls->name+"*") : ("intrusive_ptr<"+cls->name+">"))
             << " ModelEcsBase::get(int component_id)" << (isConst?" const":"") << "\n";
        body << "{\n";
        body << "    if(in_map(component_id, this->map_components_" << field << "))\n";
        body << "    {\n";
        body << "        return this->map_components_" << field << ".at(component_id);\n";
        body << "    }\n";
        body << "    return nullptr;\n";
        body << "}\n\n";
        ecs->functions.front().specific_implementations += body.str();
    }
}

std::vector<shared_ptr<Class>> GeneratorEcsCpp::getComponentClasses(Model &model) {
    std::vector<shared_ptr<Class>> out;
    for (auto &cls : model.classes) {
        if (isBased(cls, "ComponentBase") && cls->name != "ComponentBase") {
            if (cls->name == "ComponentSkillBase") out.insert(out.begin(), cls);
            else out.push_back(cls);
        }
    }
    return out;
}

void GeneratorEcsCpp::generateModelAddComponent(Model &model) {
    auto ecs = getClass(model, "ModelEcsBase");
    if (!ecs) return;

    // template <class T> void add(intrusive_ptr<T> component, int component_id=0)
    {
        Function m;
        m.name = "add";
        Object tpl; tpl.type = "T"; m.template_args.push_back(tpl);
        m.return_type.type = "void";
        // args
        Object arg0; arg0.type = "intrusive_ptr"; { Object inner; inner.type = "T"; arg0.template_args.push_back(inner); } arg0.name = "component"; m.callable_args.push_back(arg0);
        Object arg1; arg1.type = "int"; arg1.name = "component_id"; arg1.value = "0"; m.callable_args.push_back(arg1);
        ecs->functions.push_back(m);
        auto &decl1 = ecs->functions.back();

        for (auto &cls : getComponentClasses(model)) {
            auto field = componentsField(cls);
            std::ostringstream body;
            body << "template<> void ModelEcsBase::add(intrusive_ptr<" << cls->name << "> component, int component_id)\n";
            body << "{\n";
            // add for parent (single-inheritance best effort)
            if (!cls->parent_class_name.empty() && cls->parent_class_name != "ComponentBase") {
                body << "    this->add<" << cls->parent_class_name << ">(intrusive_ptr<" << cls->parent_class_name << ">(component), component_id);\n";
            }
            body << "    assert(component->id == 0 || component->id == component_id || component_id == 0);\n";
            body << "    if(component_id != 0)\n{\ncomponent->id = component_id;\n}\n";
            body << "    assert(component->id > 0);\n";
            body << "    if(in_map(component->id, this->map_components_" << field << "))\n";
            body << "    {\nlist_remove(this->components_" << field << ", this->map_components_" << field << ".at(component->id));\n}\n";
            body << "    auto iter = std::lower_bound(this->components_" << field << ".begin(), this->components_" << field << ".end(), component, [](const auto& a, const auto& b)\n{\nreturn a->id < b->id;\n});\n";
            body << "    this->components_" << field << ".insert(iter, component);\n";
            body << "    assert(std::is_sorted(this->components_" << field << ".begin(), this->components_" << field << ".end(), [](const auto& l, const auto& r)\n{\nreturn l->id < r->id;\n}));\n";
            body << "    this->map_components_" << field << "[component->id] = component;\n";
            body << "}\n\n";
            decl1.specific_implementations += body.str();
        }
    }

    // template <class T> void add(T* component, int component_id=0) -> forward to intrusive_ptr
    {
        Function m;
        m.name = "add";
        Object tpl; tpl.type = "T"; m.template_args.push_back(tpl);
        m.return_type.type = "void";
        Object arg0; arg0.type = "T"; arg0.is_pointer = true; arg0.name = "component"; m.callable_args.push_back(arg0);
        Object arg1; arg1.type = "int"; arg1.name = "component_id"; arg1.value = "0"; m.callable_args.push_back(arg1);
        ecs->functions.push_back(m);
        auto &decl2 = ecs->functions.back();
        for (auto &cls : model.classes) {
            if (!isBased(cls, "ComponentBase") || cls->name == "ComponentBase") continue;
            auto field = componentsField(cls); (void)field;
            std::ostringstream body;
            body << "template<> void ModelEcsBase::add(" << cls->name << "* component, int component_id)\n";
            body << "{\nthis->add(intrusive_ptr<" << cls->name << ">(component), component_id);\n}\n";
            decl2.specific_implementations += body.str();
        }
    }
}

void GeneratorEcsCpp::generateModelRemoveComponent(Model &model, bool useRawPointer) {
    auto ecs = getClass(model, "ModelEcsBase");
    if (!ecs) return;
    Function m;
    m.name = "remove";
    Object tpl; tpl.type = "T"; m.template_args.push_back(tpl);
    m.return_type.type = "void";
    if (useRawPointer) {
        Object arg; arg.type = "intrusive_ptr"; { Object inner; inner.type = "T"; arg.template_args.push_back(inner);} arg.name = "component"; m.callable_args.push_back(arg);
    } else {
        Object arg; arg.type = "T"; arg.is_pointer = true; arg.name = "component"; m.callable_args.push_back(arg);
    }
    ecs->functions.insert(ecs->functions.begin(), m);
    auto &decl = ecs->functions.front();

    for (auto &cls : getComponentClasses(model)) {
        auto field = componentsField(cls);
        std::ostringstream body;
        if (useRawPointer) {
            body << "template<> void ModelEcsBase::remove(" << cls->name << "* component)\n";
            body << "{\nthis->remove(intrusive_ptr<" << cls->name << ">(component));\n}\n";
        } else {
            body << "template<> void ModelEcsBase::remove(intrusive_ptr<" << cls->name << "> component)\n";
            body << "{\n";
            if (!cls->parent_class_name.empty() && cls->parent_class_name != "ComponentBase") {
                body << "    this->remove<" << cls->parent_class_name << ">(intrusive_ptr<" << cls->parent_class_name << ">(component));\n";
            }
            body << "    if(component)\n{\nlist_remove(this->components_" << field << ", component); map_remove(this->map_components_" << field << ", component->id);\n}\n";
            body << "}\n\n";
        }
        decl.specific_implementations += body.str();
    }
}

void GeneratorEcsCpp::generateModelCopyEntityFromModel(Model &model) {
    auto ecs = getClass(model, "ModelEcsBase");
    if (!ecs) return;
    auto m = makeFnDecl("fn void copy_entity_from_model(ModelEcsBase* model, int id, int new_id)");
    std::ostringstream body;
    for (auto &cls : model.classes) {
        if (!isBased(cls, "ComponentBase") || cls->name == "ComponentBase") continue;
        auto field = componentsField(cls);
        body << "auto component_" << field << " = model->get<" << cls->name << ">(id);\n";
        body << "if(component_" << field << ")\n";
        body << "{\n";
        body << "    auto clone = clone_object<" << cls->name << ">(component_" << field << ");\n";
        body << "    clone->id = new_id;\n";
        body << "    this->add<" << cls->name << ">(clone, new_id);\n";
        body << "}\n";
    }
    m.body = body.str();
    ecs->functions.push_back(std::move(m));
}

void GeneratorEcsCpp::generateModelGetComponents(Model &model, bool isConst) {
    auto ecs = getClass(model, "ModelEcsBase");

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
        if (!isBased(cls, "ComponentBase") || cls->name == "ComponentBase")
            continue;
        auto field = componentsField(cls);
        std::ostringstream body;
        
        body
        << "template <> "
        << (isConst ? "const " : "")
        << "std::vector<intrusive_ptr<"
        << cls->name
        << ">>& ModelEcsBase::get_components() "
        << (isConst ? "const" : "")
        << "\n";
        
        body << "{\nreturn this->components_" << field << "; \n}\n";
        decl.specific_implementations += body.str();
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

