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
#include "../utils/Error.hpp"
#include "cpp/GeneratorOperatorEqualsCpp.hpp"

using std::shared_ptr;
using std::string;

// --- helpers ---
std::string MODEL_ECS_TEMPLATES = R"__EACH__(
public:
    template <class TComp>
    void remove_if(const std::function<bool(TComp&)>& cond)
    {
        auto& components = this->get_components<TComp>();
        auto& map_components = this->get_map_components<TComp>();
        size_t index = -1;
        auto iter = std::remove_if(components.begin(), components.end(), [&](auto& comp)
        {
            if(cond(comp))
            {
                auto iter = map_components.find(comp.id);
                index = iter->second;
                map_components.erase(iter);
                return true;
            }
            return false;
        });
        if(iter != components.end())
        {
            components.erase(iter, components.end());
            for(auto i=index;i<components.size(); ++i)
            {
                map_components[components.at(i).id] = i;
            }
        }
    }
    
    template <class T>
    void remove_entities_with()
    {
        auto& cont = get_components<T>();
        while(!cont.empty())
        {
            this->remove_entity(cont.back().id);
        }
    }

    
    
    template <class TCompA, class TCompB>
    std::vector<std::tuple<TCompA&, TCompB&>> view()const
    {
        auto& cont_a = get_components<TCompA>();
        auto& cont_b = get_components<TCompB>();
        assert(std::is_sorted(std::begin(cont_a), std::end(cont_a), [](auto const& lhs, auto const& rhs){return lhs.id < rhs.id;}));
        assert(std::is_sorted(std::begin(cont_b), std::end(cont_b), [](auto const& lhs, auto const& rhs){return lhs.id < rhs.id;}));
        
        std::vector<std::tuple<TCompA&, TCompB&>> result;
        result.reserve(std::min(cont_a.size(), cont_b.size()));
        
        size_t i = 0, j = 0;
        auto size_a = cont_a.size();
        auto size_b = cont_b.size();
        while (i < size_a && j < size_b)
        {
            auto& comp_a = cont_a[i];
            auto& comp_b = cont_b[j];
            int ida = comp_a.id;
            int idb = comp_b.id;
            if (ida == idb)
            {
                if(!comp_a.is_removed() && !comp_b.is_removed())
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
        std::vector<std::tuple<A&, B&, Rest&...>> result;
        auto& cont_a = get_components<A>();
        result.reserve(cont_a.size());
        each<A, B, Rest...>([&](A& a, B& b, Rest&... rest)
        {
            result.emplace_back(a, b, rest...);
        });
        return result;
    }

    void break_each()
    {
        assert(this->_in_each > 0);
        this->_break_each = true;
    }
    
    template<typename A, typename Func>
    void each(Func&& func)
    {
        ++_in_each;
        auto& components = get_components<A>();
        for(auto& comp : components)
        {
            if(!comp.is_removed())
            func(comp);
            if(_break_each)
                break;
        }
        _break_each = false;
        --_in_each;
    }

    template<typename A, typename B, typename... Rest, typename Func>
    void each(Func&& func)
    {
        each_impl<A,B,Rest...>(std::index_sequence_for<A,B,Rest...>{}, std::forward<Func>(func));
    }
    
    template<typename A, typename B, typename... Rest, typename Cond, typename Func>
    void each_if(Cond&& cond, Func&& func)
    {
        each_if_impl<A,B,Rest...>(std::index_sequence_for<A,B,Rest...>{},std::forward<Func>(func), std::forward<Cond>(cond));
    }

    template <class T>
    void clear_removed()
    {
        auto& cont = get_components<T>();
        auto& map = get_map_components<T>();
        auto iter = std::remove_if(cont.begin(), cont.end(), [&](auto& component){
            if(component.is_removed())
            {
                map.erase(component.id);
                return true;
            }
            return false;
        });
        cont.erase(iter, cont.end());
    
        map.clear();
        for(size_t index=0; index<cont.size(); ++index)
        {
            map[cont[index].id] = index;
        }
    }
    
private:
    bool _break_each = false;
    int _in_each = 0;
    
    template<typename A, typename B, typename... Rest, size_t... Is, typename Func>
    void each_impl(std::index_sequence<Is...>, Func&& func)
    {
        auto containers = std::tie(get_components<A>(),
                                   get_components<B>(),
                                   get_components<Rest>()...);
        constexpr size_t N = 2 + sizeof...(Rest);
        
        std::array<size_t, N> idxs{};
        std::array<size_t, N> sizes
        {
            std::get<Is>(containers).size()...
        };
        
        _in_each++; 
        while (true)
        {
            if (((idxs[Is] >= sizes[Is]) || ...)) 
            break;
            
            std::array<int, N> ids
            {
                std::get<Is>(containers)[idxs[Is]].id...
            };
            
            int max_id = ids[0];
            for (size_t k = 1; k < N; ++k)
            {
                if (ids[k] > max_id)
                    max_id = ids[k];
            }
            
            if (((ids[Is] == max_id) && ...))
            {
                auto args = std::forward_as_tuple(std::get<Is>(containers)[idxs[Is]]...);
                if(std::apply([](auto&... comps){ return ((!comps.is_removed()) && ...); }, args))
                    std::apply([&](auto&... comps){ func(comps...); }, args);
                (++idxs[Is], ...);
            }
            else
            {
                for (size_t k = 0; k < N; ++k)
                {
                    if (ids[k] < max_id)
                    {
                        ++idxs[k];
                    }
                }
            }
            if(_break_each)
                break;
        }
        _break_each = false;
        --_in_each;
    }

    template<typename A, typename B, typename... Rest, size_t... Is, typename Func, typename Cond>
    void each_if_impl(std::index_sequence<Is...>, Func&& func, Cond&& cond)
    {
        auto containers = std::tie(get_components<A>(),
                                   get_components<B>(),
                                   get_components<Rest>()...);
        constexpr size_t N = 2 + sizeof...(Rest);
        
        std::array<size_t, N> idxs{};
        std::array<size_t, N> sizes
        {
            std::get<Is>(containers).size()...
        };

        _in_each++;
        while (true)
        {
            if (((idxs[Is] >= sizes[Is]) || ...)) break;
            
            std::array<int, N> ids
            {
                std::get<Is>(containers)[idxs[Is]].id...
            };
            
            int max_id = ids[0];
            for (size_t k = 1; k < N; ++k)
            {
                if (ids[k] > max_id) max_id = ids[k];
            }
            
            if (((ids[Is] == max_id) && ...))
            {
                auto args = std::forward_as_tuple(std::get<Is>(containers)[idxs[Is]]...);
                if(std::apply([](auto&... comps){ return ((!comps.is_removed()) && ...); }, args) &&
                   std::apply([&](auto&... comps){ return cond(comps...); }, args))
                {
                    std::apply([&](auto&... comps){ func(comps...); }, args);
                }
                (++idxs[Is], ...);
            }
            else
            {
                for (size_t k = 0; k < N; ++k)
                {
                    if (ids[k] < max_id)
                    {
                        ++idxs[k];
                    }
                }
            }
            if(_break_each)
                break;
        }
        _break_each = false;
        --_in_each;
    }
)__EACH__";

std::string ECS_SERIALIZER_XML_H = R"__(#pragma once

#include <variant>
#include <vector>
#include <string>

namespace pugi { class xml_node; }
{forward_declarations}

namespace mg 
{
namespace serializer_xml 
{

using ComponentVariant = {AllComponents};

void serialize(pugi::xml_node node, const ComponentVariant &value, const std::string &key);
void deserialize(pugi::xml_node node, ComponentVariant &value, const std::string &key);
void serialize(pugi::xml_node node, const std::vector<ComponentVariant> &value, const std::string &key);
void deserialize(pugi::xml_node node, std::vector<ComponentVariant> &value, const std::string &key);

} // namespace serializer_xml
} // namespace mg
)__";

std::string ECS_SERIALIZER_XML_CPP = R"__(#include "{header}"

#include <string>
#include "pugixml/pugixml.hpp"
{includes}

namespace mg
{
namespace serializer_xml
{

template <class T>
bool deserialize_variant_value(pugi::xml_node node, ComponentVariant &value, const std::string &type)
{
    if (type != T::TYPE)
        return false;

    value.emplace<T>();
    std::get<T>(value).deserialize_xml(node);
    return true;
}

void serialize(pugi::xml_node node, const ComponentVariant &value, const std::string &key)
{
    pugi::xml_node child = key.empty() ? node : node.append_child(key.c_str());
    std::visit([&child](const auto &component)
    {
        child.append_attribute("type").set_value(component.get_type().c_str());
        component.serialize_xml(child);
    }, value);
}

void deserialize(pugi::xml_node node, ComponentVariant &value, const std::string &key)
{
    pugi::xml_node child = key.empty() ? node : node.child(key.c_str());
    const std::string type = child.attribute("type").as_string();

    {deserialize_components}
}

void serialize(pugi::xml_node node, const std::vector<ComponentVariant> &value, const std::string &key)
{
    pugi::xml_node child = key.empty() ? node : node.append_child(key.c_str());
    for (const auto &item : value)
    {
        serialize(child.append_child("component"), item, "");
    }
}

void deserialize(pugi::xml_node node, std::vector<ComponentVariant> &value, const std::string &key)
{
    pugi::xml_node child = key.empty() ? node : node.child(key.c_str());
    value.clear();
    for (pugi::xml_node item : child)
    {
        ComponentVariant component;
        deserialize(item, component, "");
        value.push_back(std::move(component));
    }
}

} // namespace serializer_xml
} // namespace mg
)__";

std::string ECS_SERIALIZER_JSON_H = R"__(#pragma once

#include <variant>
#include <vector>
#include <string>

namespace Json { class Value; }
{forward_declarations}

namespace mg
{
namespace serializer_json
{

using ComponentVariant = {AllComponents};

void serialize(Json::Value &node, const ComponentVariant &value, const std::string &key);
void deserialize(Json::Value &node, ComponentVariant &value, const std::string &key);
void serialize(Json::Value &node, const std::vector<ComponentVariant> &value, const std::string &key);
void deserialize(Json::Value &node, std::vector<ComponentVariant> &value, const std::string &key);

} // namespace serializer_json
} // namespace mg
)__";

std::string ECS_SERIALIZER_JSON_CPP = R"__(#include "{header}"

#include <string>
#include "jsoncpp/value.h"
{includes}

namespace mg
{
namespace serializer_json
{

template <class T>
bool deserialize_variant_value(Json::Value &node, ComponentVariant &value, const std::string &type)
{
    if (type != T::TYPE)
        return false;

    value.emplace<T>();
    std::get<T>(value).deserialize_json(node);
    return true;
}

void serialize(Json::Value &node, const ComponentVariant &value, const std::string &key)
{
    Json::Value &child = key.empty() ? node : node[key];
    std::visit([&child](const auto &component)
    {
        child["type"] = component.get_type();
        component.serialize_json(child);
    },value);
}

void deserialize(Json::Value &node, ComponentVariant &value, const std::string &key)
{
    Json::Value &child = key.empty() ? node : node[key];
    const std::string type = child["type"].asString();
    {deserialize_components}
}

void serialize(Json::Value &node, const std::vector<ComponentVariant> &value, const std::string &key)
{
    Json::Value &child = key.empty() ? node : node[key];
    for (const auto &item : value)
    {
        Json::Value serialized_item(Json::objectValue);
        serialize(serialized_item, item, "");
        child.append(serialized_item);
    }
}

void deserialize(Json::Value &node, std::vector<ComponentVariant> &value, const std::string &key)
{
    Json::Value &child = key.empty() ? node : node[key];
    value.clear();
    for (Json::Value &item : child)
    {
        ComponentVariant component;
        deserialize(item, component, "");
        value.push_back(std::move(component));
    }
}

} // namespace serializer_json
} // namespace mg
)__";



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


GeneratorEcsCpp::GeneratorEcsCpp(const FeatureEcs &feature) {
    _ecs_model_base_name = feature.model_base;
    _ecs_component_base_name = feature.component_base;
    _ecs_list_components = feature.list_components;
    _discard_inheritance = feature.discard_inheritance;
    
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
    auto ecsBase = model.get_class(_ecs_model_base_name);
    if (!ecsBase){
        std::cout << "Cannot find ECS model base class: " << _ecs_model_base_name << std::endl;
        return;
    }
    
    createPimplClass(model, ecsBase);
    createPimplMember(model, ecsBase);
    
    auto ecsPimpl = model.get_class("EcsPimplImpl");
    
    generateComponentSystemMembers(model);
    generateContainers(model, ecsPimpl);
    generateClearComponents(model, ecsBase);

    generateModelAddComponent(model);
    generateModelRemoveComponent(model);
    generateModelGetComponent(model, /*isConst*/ true);
    generateModelGetComponent(model, /*isConst*/ false);
//    generateModelCopyEntityFromModel(model);
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
    generateFactory(model);
    changeListEcsComponents(model);
    addSerializeListEcsComponents(model);
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
    auto system = model.get_class("SystemSkillBase");
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
        replace_all(block, "@(get_components)", "get_components<" + skill->name + ">()");
        replace_all(block, "@(components)", list_name);
        if (!method->body.empty())
            method->body += "\n";
        method->body += block;
        system->user_includes.insert(skill->name);
    }
}

void GeneratorEcsCpp::generate_model_method_save_skills(Model &model) {
    auto controller = model.get_class("ControllerDungeonBase");
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

    auto info = model.get_class("ModelDungeonBaseChangeHeroesInfo");
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
            std::string member = format_indexes(R"(hash_map<string, {0}*> {1})", skill->name, field);
            info->members.push_back(parse_object(member, true));
        }
    }
}

void GeneratorEcsCpp::modifySources(Model &model,
                                    const std::shared_ptr<Class> &cls,
                                    std::string &header, std::string &source) {
    if (cls->name == _ecs_model_base_name) {
        replace_all(header, "};", MODEL_ECS_TEMPLATES + "\n};");
    }

    if (_ecs_list_components.empty())
        return;

    bool uses_ecs_components_variant = false;
    for (const auto &member : cls->members) {
        if (member.type == "list" && !member.template_args.empty() &&
            member.template_args.front().type == "std::variant") {
            uses_ecs_components_variant = true;
            break;
        }
        if (member.type == "std::variant") {
            uses_ecs_components_variant = true;
            break;
        }
    }
    if (!uses_ecs_components_variant)
        return;

    auto ecsModel = model.get_class(_ecs_model_base_name);
    std::string header_path;
    if (ecsModel && !ecsModel->group.empty())
        header_path = ecsModel->group + "/";
    header_path += "ecs_serializer_";

    if (model.config.serializeFormats & static_cast<int>(SerializeFormat::Json)) {
        replace_all(source, "#include \"SerializerJson.h\"", "#include \"SerializerJson.h\"\n#include \"" + header_path + "json.h\"");
        replace_all(source, "#include \"../SerializerJson.h\"", "#include \"../SerializerJson.h\"\n#include \"" + header_path + "json.h\"");
    }
    if (model.config.serializeFormats & static_cast<int>(SerializeFormat::Xml)) {
        replace_all(source, "#include \"SerializerXml.h\"", "#include \"SerializerXml.h\"\n#include \"" + header_path + "xml.h\"");
        replace_all(source, "#include \"../SerializerXml.h\"", "#include \"../SerializerXml.h\"\n#include \"" + header_path + "xml.h\"");
    }
}

void GeneratorEcsCpp::createPimplClass(Model &model, const std::shared_ptr<Class> &ecsBase){
    auto pimpl = std::make_shared<Class>();
    pimpl->name = "EcsPimpl";
    pimpl->type = "class";
    pimpl->group = ecsBase->group;
    pimpl->discard_copy = true;
    pimpl->discard_copy_ctr = true;
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

        std::string decl = format_indexes(R"(list<{0}> components_{1})", cls->name, field);
        ecsBase->members.push_back(parse_object(decl, true));

        decl = format_indexes( "hash_map<int, uint64_t>:runtime map_components_{1}", cls->name, field);
        ecsBase->members.push_back(parse_object(decl, true));
    }
}


void GeneratorEcsCpp::generateClearComponents(Model &model, const std::shared_ptr<Class> &ecsBase){

    auto fn = parse_function("fn<T> void clear()");
    
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
    if(!ecs->has_method("update_systems"))
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
            auto sysCls = model.get_class(sys);
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

void GeneratorEcsCpp::generateBuildMaps(Model &model) {
    auto ecs = model.get_class(_ecs_model_base_name);
    if (!ecs)
        return;
    auto fn = parse_function("fn void build_maps()");
    std::string body;
    body += "auto impl = static_cast<EcsPimplImpl*>(this->_pimpl.ptr());";
    
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        body += format_indexes(R"(
for(size_t index = 0; index < impl->components_{0}.size(); ++index)
{
    impl->map_components_{0}[impl->components_{0}.at(index).id] = index;
}
)", field);
    }
    fn.body = body;
    ecs->functions.push_back(std::move(fn));
}

void GeneratorEcsCpp::generateRemoveEntity(Model &model) {
    auto ecs = model.get_class(_ecs_model_base_name);
    if (!ecs)
        return;
    Function *fn = nullptr;
    for (auto &f : ecs->functions)
        if (f.name == "remove_entity") {
            fn = &f;
            break;
        }
    if (!fn){
        auto f = parse_function("fn void remove_entity(int id)");
        ecs->functions.push_back(std::move(f));
        fn = &ecs->functions.back();
    }
    std::string add;
    add += "auto impl = static_cast<EcsPimplImpl*>(this->_pimpl.ptr());";
    if (!fn->body.empty())
        add += fn->body + "\n";
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        add += format_indexes(R"(
if(auto iter = impl->map_components_{0}.find(id); iter != impl->map_components_{0}.end())
{
    auto& component = impl->components_{0}.at(iter->second);
    this->remove<{1}>(component);
}
)", field, cls->name);
    }
    fn->body = add;
}

void GeneratorEcsCpp::generateModelGetComponent(Model &model, bool isConst) {
    auto ecs = model.get_class(_ecs_model_base_name);
    if (!ecs)
        return;
    Function method;
    method.name = "get";
    // template <class T>
    Object tpl;
    tpl.type = "T";
    method.template_args.push_back(tpl);
    // return type
    method.return_type.type = "T";
    method.return_type.is_ref = true;
    if (isConst) {
        method.return_type.is_const = true;
    }
    // args
    method.callable_args.push_back(parse_object("int component_id", true));
    method.is_const = isConst;
    ecs->functions.insert(ecs->functions.begin(), method);

    // specializations
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        std::string retType = isConst ? ("const " + cls->name + "&")
                                      : (cls->name + "&");
        std::string spec;
        spec += format_indexes(R"(template<> {0} {1}::get(int component_id){2}
{
auto impl = static_cast<{2} EcsPimplImpl*>(this->_pimpl.ptr());
auto iter = impl->map_components_{3}.find(component_id);
if(iter != impl->map_components_{3}.end())
{
    auto index = iter->second;
    assert(impl->components_{3}.at(index).id == component_id);
    auto& result = impl->components_{3}.at(index);
    if(!result.is_removed()) return result;
}
static {4} dummy;
dummy.id = -1;
return dummy;
})", retType, _ecs_model_base_name, (isConst ? " const" : ""), field, cls->name);
        ecs->functions.front().specific_implementations += spec;
    }
}

std::vector<shared_ptr<Class>> GeneratorEcsCpp::getComponentClasses(Model &model) {
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
    auto ecs = model.get_class(_ecs_model_base_name);
    if (!ecs)
        return;

    Function m = parse_function("fn<T> void add(T&& component, int component_id=0)");

    for (auto &cls : getComponentClasses(model)) {
        auto field = componentsField(cls);
        m.specific_implementations += format_indexes(R"(template<> void {0}::add({1}&& component, int component_id)
    {
    assert(component.id == 0 || component.id == component_id || component_id == 0);
    if(component_id != 0)
    {
        component.id = component_id;
    }
    assert(component.id > 0);

    auto impl = static_cast<EcsPimplImpl*>(this->_pimpl.ptr());
    auto iter = impl->map_components_{2}.find(component.id);
    auto& components = impl->components_{2};
    size_t new_index = -1;
    bool was_shift = false;
    if(iter != impl->map_components_{2}.end())
    {
        new_index = iter->second;
        components.at(new_index) = std::move(component);
    } 
    else 
    {
        auto iter = std::lower_bound(components.begin(), components.end(), component, [](const auto& a, const auto& b)
        {
            return a.id < b.id;
        });
        new_index = iter - components.begin();
        was_shift = true;
        components.insert(iter, std::move(component));
    }

    assert(std::is_sorted(components.begin(), components.end(), [](const auto& l, const auto& r)
    {
        return l.id < r.id;
    }));
    if(was_shift)
    {
        for(size_t i=new_index; i<components.size(); ++i)
        {
            impl->map_components_{2}[components.at(i).id] = i;
        }
    }
    })", _ecs_model_base_name, cls->name, field);
    }
    ecs->functions.push_back(std::move(m));
}

void GeneratorEcsCpp::generateModelRemoveComponent(Model &model) {
    auto ecs = model.get_class(_ecs_model_base_name);
    if (!ecs)
        return;
    Function m = parse_function("fn<T> void remove(T& component, bool clean=true)");

//    for (auto &cls : getComponentClasses(model)) {
//        auto field = componentsField(cls);
    m.body += R"__(
    component.mark_removed();
    if(clean) clear_removed<T>();
    )__";
//    }
    ecs->functions.insert(ecs->functions.begin(), std::move(m));
}

void GeneratorEcsCpp::generateModelCopyEntityFromModel(Model &model) {
    auto ecs = model.get_class(_ecs_model_base_name);
    if (!ecs)
        return;
    auto m = parse_function("fn void copy_entity_from_model(" + _ecs_model_base_name + "* model, int id, int new_id)");
    
    std::string body;
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) ||
            cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        if(cls->is_discard_copy_ctr()){
            if(model.config.serializeFormats & static_cast<int>(SerializeFormat::Xml))
                body += format_indexes(R"(
                if(auto& component_{0} = model->get<{1}>(id))
                {
                    pugi::xml_node node;
                    component_{0}.serialize_xml(node);
                    {1} clone;
                    clone.deserialize_xml(node);
                    clone.id = new_id;
                    this->add<{1}>(std::move(clone));
                })", field, cls->name);
            else
                body += format_indexes(R"(
                if(auto& component_{0} = model->get<{1}>(id))
                {
                    Json::Value node;
                    component_{0}.serialize_json(node);
                    {1} clone;
                    clone.deserialize_json(node);
                    clone.id = new_id;
                    this->add<{1}>(std::move(clone));
                })", field, cls->name);
        } else {
            body += format_indexes(R"(
            if(auto& component_{0} = model->get<{1}>(id))
            {
                {1} clone = component_{0};
                clone.id = new_id;
                this->add<{1}>(std::move(clone));
            })", field, cls->name);
        }
    }
    m.body = body;
    ecs->functions.push_back(std::move(m));
}

void GeneratorEcsCpp::generateModelGetComponents(Model &model, bool isConst) {
    auto ecs = model.get_class(_ecs_model_base_name);

    Function m = parse_function("fn<T> list<T>:ref get_components()");
    m.return_type.is_const = isConst;
    m.is_const = isConst;
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) || cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        m.specific_implementations += format_indexes(R"(
        template <> {0}std::vector<{1}>& {2}::get_components() {3}
        {
            return static_cast<{0}EcsPimplImpl*>(this->_pimpl.ptr())->components_{4};;
        }
        )", (isConst ? std::string("const ") : std::string("")), cls->name, _ecs_model_base_name, (isConst ? std::string("const") : std::string("")), field);
    }
    ecs->functions.insert(ecs->functions.begin(), std::move(m));
}

void GeneratorEcsCpp::generateModelGetMapComponents(Model &model, bool isConst) {
    auto ecs = model.get_class(_ecs_model_base_name);

    auto m = parse_function(format_indexes("fn<T> hash_map<int, uint64_t>:ref{0} get_map_components():protected{0}", isConst ? ":const" : ""));
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) || cls->name == _ecs_component_base_name)
            continue;
        auto field = componentsField(cls);
        m.specific_implementations += format_indexes(R"(
        template <>{3} std::unordered_map<int, uint64_t>& {0}::get_map_components<{1}>(){3}
        {
            return static_cast<{3} EcsPimplImpl*>(this->_pimpl.ptr())->map_components_{2};
        }
        )", _ecs_model_base_name, cls->name, field, isConst ? " const" : "");
    }
    ecs->functions.insert(ecs->functions.begin(), std::move(m));
}

void GeneratorEcsCpp::generateComponentSystemMembers(Model &model){
    auto base = model.get_class(_ecs_component_base_name);
    base->members.push_back(parse_object("int:private _system_flags = 0", true));
    base->members.push_back(parse_object("int:const:static FLAG_REMOVED = 0x1", true));
    
    base->functions.push_back(parse_function(R"(fn bool is_removed():const { return this->_system_flags & FLAG_REMOVED; })"));
    base->functions.push_back(parse_function(R"(fn void mark_removed() { this->_system_flags |= FLAG_REMOVED; })"));
    base->functions.push_back(parse_function(R"(fn void mark_unremoved() { this->_system_flags &= ~FLAG_REMOVED; })"));
    
    Function cast_to_bool;
    cast_to_bool.is_const = true;
    cast_to_bool.name = "operator bool";
    cast_to_bool.body = "return id > 0 && !is_removed();";
    base->functions.push_back(std::move(cast_to_bool));
    
    for (auto &cls : model.classes) {
        if (!isBased(cls, _ecs_component_base_name) || cls->name == _ecs_component_base_name)
            continue;
        GeneratorOperatorEqualsCpp::generate_signaturs(cls);
        cls->get_copy_operator()->access = AccessSpecifier::m_private;
        for(auto& f : cls->functions){
            if(f.name == cls->name && f.callable_args.size() == 1 && f.callable_args[0].type == cls->name && f.callable_args[0].is_ref && !f.callable_args[0].is_rvalue)
                f.access = AccessSpecifier::m_private;
        }
        
        cls->functions.push_back(parse_function(format_indexes(R"(fn {0} copy():const { return *this; })", cls->name)));
    }
}

void GeneratorEcsCpp::generateFactory(Model &model){
    auto ecs = model.get_class(_ecs_model_base_name);
    
    if(model.config.serializeFormats & static_cast<int>(SerializeFormat::Xml)) {
        auto fn = parse_function("fn bool add_component_from_xml(pugi::xml_node node, int id)");
        fn.body += "auto name = node.name();\n";
        fn.body += build_all_components(model) + " component;\n";
        ecs->functions.push_back(std::move(fn));
    }
    auto fn = ecs->get_method("add_component_from_xml");
    fn->body += "bool result = true;\nif(0){}\n";
    for (auto &cls : model.classes) {
        if (isBased(cls, _ecs_component_base_name) && cls->name != _ecs_component_base_name){
            fn->body += "else if(name == " + cls->name + "::TYPE) \ncomponent = " + cls->name + "();\n";
        }
    }
    fn->body += "else result = false;\n";
    if(model.config.serializeFormats & static_cast<int>(SerializeFormat::Xml)) {
        fn->body += R"_(
        std::visit([&](auto& component)
        {
            component.deserialize_xml(node);
            this->add(std::move(component), id);
        }, component);
        )_";
    }
    fn->body += "return result;";
}

std::string GeneratorEcsCpp::build_all_components(Model& model)
{
    std::string all_components = "std::variant<";
    for (auto &cls : model.classes) {
        if (isBased(cls, _ecs_component_base_name) && cls->name != _ecs_component_base_name){
            all_components += cls->name + ",";
        }
    }
    all_components.pop_back();
    all_components += ">";
    return all_components;
}

std::string GeneratorEcsCpp::build_list_all_components(Model& model)
{
    std::string all_components = "std::variant<";
    for (auto &cls : model.classes) {
        if (isBased(cls, _ecs_component_base_name) && cls->name != _ecs_component_base_name){
            all_components += cls->name + ",";
        }
    }
    all_components.pop_back();
    all_components += ">";
    return "list<" + all_components + ">";
}

void GeneratorEcsCpp::changeListEcsComponents(Model &model){
    if(_ecs_list_components.empty()){
        return;
    }
    
    auto list_all_components = parse_object(build_list_all_components(model));
    for(auto& cls : model.classes){
        for(auto& member : cls->members){
            if(member.type == _ecs_list_components){
                auto name = member.name;
                member = list_all_components;
                member.name = std::move(name);
            }
        }
    }
}

void GeneratorEcsCpp::addSerializeListEcsComponents(Model &model){
    if(_ecs_list_components.empty()){
        return;
    }
    
    auto ecsModel = model.get_class(_ecs_model_base_name);
    std::string header;
    if(!ecsModel->group.empty())
        header = ecsModel->group + "/";
    header += "ecs_serializer_";
    
    std::string all_components = build_all_components(model);
    std::string deserialize;
    std::string includes;
    std::string forward_declarations = "namespace mg\n{\n";
    for (auto &cls : model.classes) {
        if (isBased(cls, _ecs_component_base_name) && cls->name != _ecs_component_base_name){
            deserialize += "deserialize_variant_value<" + cls->name + ">(child, value, type) ||";
            includes += "#include \"" + (cls->group.empty() ? "" : cls->group + "/") + cls->name + ".h\"\n";
            forward_declarations += "class " + cls->name + ";\n";
        }
    }
    forward_declarations += "} // namespace mg\n";
    
    deserialize.pop_back();
    deserialize.pop_back();
    deserialize.pop_back();
    deserialize += ";";
    
    if(model.config.serializeFormats & static_cast<int>(SerializeFormat::Xml)) {
        auto header_data = ECS_SERIALIZER_XML_H;
        replace_all(header_data, "{forward_declarations}", forward_declarations);
        replace_all(header_data, "{AllComponents}", all_components);
        model.addFile(nullptr, header + "xml.h", header_data);

        auto cpp_data = ECS_SERIALIZER_XML_CPP;
        replace_all(cpp_data, "{header}", header + "xml.h");
        replace_all(cpp_data, "{includes}", includes);
        replace_all(cpp_data, "{deserialize_components}", deserialize);
        model.addFile(nullptr, header + "xml.cpp", cpp_data);
    }
    if(model.config.serializeFormats & static_cast<int>(SerializeFormat::Json)) {
        auto header_data = ECS_SERIALIZER_JSON_H;
        replace_all(header_data, "{forward_declarations}", forward_declarations);
        replace_all(header_data, "{AllComponents}", all_components);
        model.addFile(nullptr, header + "json.h", header_data);

        auto cpp_data = ECS_SERIALIZER_JSON_CPP;
        replace_all(cpp_data, "{header}", header + "json.h");
        replace_all(cpp_data, "{includes}", includes);
        replace_all(cpp_data, "{deserialize_components}", deserialize);
        model.addFile(nullptr, header + "json.cpp", cpp_data);
    }
}
