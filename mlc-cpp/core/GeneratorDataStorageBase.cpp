//
//  GeneratorDataStorageBase.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorDataStorageBase.hpp"


#include "Class.hpp"
#include "Object.hpp"
#include "Function.hpp"
#include "Parser.hpp"
#include "Model.hpp"
#include "Common.hpp"
//#include "SerializeFormat.hpp"
#include "Object.hpp"
//#include "AccessSpecifier.hpp"

// Утилиты

std::string getDataName(const std::string& name) {
    std::string s = name;
    if (s.rfind("Data", 0) == 0) {
        s.erase(0, 4);
    }
    std::string out;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (std::isupper(c) && i > 0) out.push_back('_');
        out.push_back(static_cast<char>(std::tolower(c)));
    }
    return out;
}

std::string getClassNameFromDataName(const std::string& name) {
    bool upper = true;
    std::string out;
    for (char c : name) {
        if (c == '_') {
            upper = true;
            continue;
        }
        out.push_back( upper ? static_cast<char>(std::toupper(c)) : c );
        upper = false;
    }
    return "Data" + out;
}

static std::string pluralizeWord(const std::string& w)
{
    if (w.empty()) return w;
    static const std::unordered_map<std::string,std::string> irregular = {
        {"child","children"}, {"person","people"}, {"man","men"}, {"woman","women"},
        {"mouse","mice"}, {"louse","lice"}, {"goose","geese"}, {"tooth","teeth"}, {"foot","feet"},
        {"cactus","cacti"}, {"focus","foci"}, {"fungus","fungi"}, {"nucleus","nuclei"}, {"radius","radii"},
        {"analysis","analyses"}, {"basis","bases"}, {"crisis","crises"}, {"thesis","theses"}, {"synthesis","syntheses"},
        {"diagnosis","diagnoses"}, {"ellipsis","ellipses"}, {"parenthesis","parentheses"}, {"oasis","oases"},
        {"phenomenon","phenomena"}, {"criterion","criteria"}, {"index","indices"}, {"matrix","matrices"},
        {"vertex","vertices"}, {"axis","axes"}, {"ox","oxen"}, {"quiz","quizzes"}
    };
    if (auto it = irregular.find(w); it != irregular.end())
        return it->second;
    auto ends_with = [&](const std::string& suf){
        return w.size() >= suf.size() && w.compare(w.size()-suf.size(), suf.size(), suf) == 0;
    };
    auto is_vowel = [](char c){ c = (char)std::tolower((unsigned char)c); return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'; };
    if (w.size()>=2 && w.back()=='y' && !is_vowel(w[w.size()-2]))
        return w.substr(0, w.size()-1) + "ies";
    if (ends_with("s") || ends_with("x") || ends_with("z") || ends_with("ch") || ends_with("sh"))
        return w + "es";
    static const std::unordered_set<std::string> f_exceptions = {"roof","belief","chef","chief","cliff","proof","reef","safe","gulf"};
    if (ends_with("fe"))
        return w.substr(0, w.size()-2) + "ves";
    if (ends_with("f") && !f_exceptions.count(w))
        return w.substr(0, w.size()-1) + "ves";
    static const std::unordered_set<std::string> o_es = {"hero","potato","tomato","echo","veto","torpedo","embargo"};
    if (ends_with("o"))
        return (o_es.count(w) ? w + "es" : w + "s");
    return w + "s";
}

std::string getDataListName(const std::string& name) {
    if (name.empty()) return {};
    auto pos = name.rfind('_');
    if (pos == std::string::npos)
        return pluralizeWord(name);
    return name.substr(0, pos+1) + pluralizeWord(name.substr(pos+1));
}

// Сам класс

GeneratorDataStorageBase::GeneratorDataStorageBase() {
    _model = nullptr;
}

void GeneratorDataStorageBase::generate(Model &model) {
    _model = &model;
    _class = std::make_shared<Class>();

    // настраиваем себя как класс
    _class->type = "class";
    _class->name = "DataStorage";
    _class->is_serialized = true;
    _class->prefer_use_forward_declarations = true;

    // создаём контейнеры для каждого storage-класса
    for (auto &cls : _model->classes) {
        if (!cls->is_storage)
            continue;
        if (!_model->is_side(cls->side))
            continue;

        Object obj;
        obj.type  = "map";
        obj.name  = getDataListName(getDataName(cls->name));
        obj.template_args.push_back(Object("string",""));
        obj.template_args.push_back(Object(cls->name,""));
        obj.access = AccessSpecifier::m_private;
        _class->members.push_back(obj);
        dataMembers_[cls->name] = obj;

        createKeysGetter(obj.name);
    }

    // флаг «загружено»
    {
        Object loaded;
        loaded.is_runtime = true;
        loaded.name = "_loaded";
        loaded.type = "bool";
        loaded.value = "false";
        loaded.access = AccessSpecifier::m_private;
        _class->members.push_back(loaded);
    }

    // общий shared() и instance
    createSharedMethod();

    // методы инициализации
    if (_model->config.serializeFormats & static_cast<int>(SerializeFormat::Xml)) {
        addInitializeFunctionXml();
    }
    if (_model->config.serializeFormats & static_cast<int>(SerializeFormat::Json)) {
        addInitializeFunctionJson();
    }

    // геттеры
    createGetters(_model->classes);
    createGettersMaps(_model->classes);

    // регистрируем себя в модели
    _model->add_class(_class);
}

void GeneratorDataStorageBase::addInitializeFunctionXml() {
    Function m;
    m.name       = "initialize_xml";
    m.return_type = Objects::VOID;
    m.is_const    = true;
    m.callable_args.emplace_back(Object("string", "content"));
    m.translated = true;
    m.body += getInitializeFunctionXmlBody();
    _class->functions.push_back(std::move(m));
}

void GeneratorDataStorageBase::addInitializeFunctionJson() {
    Function m;
    m.name       = "initialize_json";
    m.return_type = Objects::VOID;
    m.is_const    = true;
    m.callable_args.emplace_back(Object("string", "content"));
    m.translated = true;
    m.body += (getInitializeFunctionJsonBody());
    _class->functions.push_back(std::move(m));
}

void GeneratorDataStorageBase::createSharedMethod() {
    if (isNeedCreateStaticInstance()) {
        Object inst;
        inst.type    = _class->name;
        inst.name    = "__instance";
        inst.is_static= true;
        inst.is_pointer = true;
        inst.access  = AccessSpecifier::m_private;
        _class->members.push_back(inst);
    }

    Function m;
    m.name       = "shared";
    m.return_type = parse_object(_class->name + ":ref:const");
    m.is_static   = true;
    m.translated = true;
    // разбиваем тело по строкам
    m.body += getSharedMethodBody();
    _class->functions.push_back(std::move(m));
}

void GeneratorDataStorageBase::createGetters(const std::vector<std::shared_ptr<Class>>& classes) {
    for (auto &cls : classes) {
        if (!cls->is_storage)
            continue;
        if (!_model->is_side(cls->side))
            continue;

        std::string mapName = getDataListName(getDataName(cls->name));
        Function m;
        m.name       = "get" + cls->name;
        m.return_type = Objects::VOID;
        m.callable_args.emplace_back(Object("void", "name"));

        std::string body = getPatternGetter();
        replace_all(body, "{map}", mapName);
        replace_all(body, "{type}", cls->name);
        m.body += (body);
        m.translated = true;
        _class->functions.push_back(std::move(m));
    }
}

void GeneratorDataStorageBase::createKeysGetter(const std::string& mapName) {
    Function m;
    m.name       = "get_" + mapName + "_keys";
    m.is_const    = true;
    m.return_type = parse_object("list<string>");
    m.body += R"(
        std::vector<std::string> result;
        for (auto&& [key, _] : this->)" + mapName + R"() 
        {
            list_push(result, key);
        }
        return result;
    )";
    _class->functions.push_back(std::move(m));
}

void GeneratorDataStorageBase::createGettersMaps(const std::vector<std::shared_ptr<Class>>& classes) {
    for (auto &cls : classes) {
        if (!cls->is_storage)
            continue;
        if (!_model->is_side(cls->side))
            continue;

        std::string mapName = getDataListName(getDataName(cls->name));
        Function m;
        m.name       = "get_" + mapName;
        m.is_const    = true;
        m.return_type = parse_object("map<string, " + cls->name + ">:const:ref");
        m.body += "\nreturn this->" + mapName + ";\n";
        _class->functions.push_back(std::move(m));
    }
}
