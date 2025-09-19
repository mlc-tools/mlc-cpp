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

std::string getDataListName(const std::string& name) {
    if (name.empty()) return {};
    char last = name.back();
    std::string s = name;
    auto vowel = [](char c){
        return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||c=='y';
    };
    if (last=='y') {
        if (vowel(last)) s = s.substr(0,s.size()-1)+"ies";
        else           s += 's';
    }
    else if (last=='o' || last=='u') {
        s += "es";
    }
    else if (last=='f') {
        s = s.substr(0,s.size()-1)+"ves";
    }
    else if (s.size()>1 && s.substr(s.size()-2)=="fe") {
        s = s.substr(0,s.size()-2)+"ves";
    }
    else if ( s=="s" || s=="ss" || s=="x" || s=="sh" || s=="ch" ) {
        s += "es";
    }
    else {
        s += 's';
    }
    return s;
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
//    if (_model->serializeFormats & static_cast<int>(SerializeFormat::xml)) {
        addInitializeFunctionXml();
//    }
//    if (_model->serializeFormats & static_cast<int>(SerializeFormat::json)) {
//        addInitializeFunctionJson();
//    }

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
    m.body += (
        "std::vector<std::string> result;\n"
        "for (auto&& [key, _] : this->" + mapName + ") {\n"
        "    list_push(result, key);\n"
        "}\n"
        "return result;"
    );
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
        m.body += ("return this->" + mapName + ";");
        _class->functions.push_back(std::move(m));
    }
}
