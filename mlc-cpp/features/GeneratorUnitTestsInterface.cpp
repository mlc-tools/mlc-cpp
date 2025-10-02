//
//  GeneratorUnitTestsInterface.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 29.07.2025.
//
#include "GeneratorUnitTestsInterface.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

#include "Class.hpp"
#include "Common.hpp"
#include "Error.hpp"
#include "Function.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "Parser.hpp"

using namespace std;

const map<string, int> GeneratorUnitTestsInterface::ASSERTS = {
    {"this->assertTrue(", 1},    {"this->assertFalse(", 1},
    {"this->assertEqual(", 2},   {"this->assertNotEqual(", 2},
    {"this->assertNull(", 1},    {"this->assertNotNull(", 1},
    {"this->assertInMap(", 2},   {"this->assertNotInMap(", 2},
    {"this->assertInList(", 2},  {"this->assertNotInList(", 2},
    {"this->assertInRange(", 3}, {"this->assertNotInRange(", 3},
};

const string GeneratorUnitTestsInterface::BASE_CLASSES = R"(
class tests/Logger:test:virtual
{
    fn void message(string message):abstract
    fn void log(string message)
    {
        this->message(message);
    }
}

class tests/TestCase:test
{
    Logger*:runtime logger
    bool:runtime result = true

    fn void initialize(Logger* logger)
    {
        this->logger = logger;
    }
    fn void execute():abstract
    fn void add_result(bool result, string message):private
    {
        if(this->result && !result)
        {
            this->logger->message(this->get_type() + ":");
        }
        if(!result)
        {
            this->logger->message(" - Failed: " + message);
        }
        this->result = this->result && result;
    }
    fn void assertTrue(bool expression, string message="")
    {
        this->add_result(expression, message);
    }
    fn void assertFalse(bool expression, string message="")
    {
        this->add_result(!expression, message);
    }
    fn<L, R> void assertEqual(L left, R right, string message="")
    {
        this->add_result(left == right, message);
    }
    fn<L, R> void assertNotEqual(L left, R right, string message="")
    {
        this->add_result(left != right, message);
    }
    fn<T> void assertNull(T value, string message="")
    {
        this->add_result(value == nullptr, message);
    }
    fn<T> void assertNotNull(T value, string message="")
    {
        this->add_result(value != nullptr, message);
    }
    function<Key, Value> void assertInMap(Key key, map<Key, Value>:const:ref map, string message="")
    {
        this->add_result(in_map(key, map), message);
    }
    function<Key, Value> void assertNotInMap(Key key, map<Key, Value>:const:ref map, string message="")
    {
        this->add_result(!in_map(key, map), message);
    }
    fn<T> void assertInList(T item, list<T>:const:ref list, string message="")
    {
        this->add_result(in_list(item, list), message);
    }
    fn<T> void assertNotInList(T item, list<T>:const:ref list, string message="")
    {
        this->add_result(!in_list(item, list), message);
    }
    fn<T> void assertInRange(T value, T min_value, T max_value, string message="")
    {
        bool result = value >= min_value && value <= max_value;
        this->add_result(result, message);
    }
    fn<T> void assertNotInRange(T value, T min_value, T max_value, string message="")
    {
        bool result = value < min_value || value > max_value;
        this->add_result(result, message);
    }
}
)";

void GeneratorUnitTestsInterface::generate(Model &model) {
    if (!model.config.generate_tests)
        return;
    _model = &model;

    vector<shared_ptr<Class>> test_classes;
    for (auto &cls : model.classes) {
        if (auto t = generateTestInterface(cls)) {
            test_classes.push_back(t);
        }
    }

    // 2) Базовые классы TestCase и Logger
    generateBaseClasses();

    model.add_classes(test_classes);
    model.add_class(generateAllTestsClass());
}

void GeneratorUnitTestsInterface::generateBaseClasses() {
    // Просто пропускаем BASE_CLASSES через парсер
    Parser parser(*_model);
    parser.parseText(BASE_CLASSES);
}

shared_ptr<Class> GeneratorUnitTestsInterface::generateTestInterface(
    const shared_ptr<Class> &cls) {
    // Пропускаем уже тестовые интерфейсы сами
    if (cls->name.find("ITest", 0) == 0)
        return nullptr;

    if (cls->name == "NullishOperator")
        std::cout << "";

    string test_interface_name = "ITest" + cls->name;
    // нет наследников — пропускаем
    auto subs = _model->getSubclassesOfClass(test_interface_name);
    if (subs.empty())
        return nullptr;

    auto i_test_interface = make_shared<Class>();
    i_test_interface->type = "class";
    i_test_interface->name = test_interface_name;
    i_test_interface->group = "tests";
    i_test_interface->parent_class_name = "TestCase";

    // Для каждого метода оригинального класса
    for (auto &f : cls->functions) {
        if ((f.name != "visit" && f.name != "accept") &&
            f.access == AccessSpecifier::m_public) {
            std::string name = f.name;
            if (name.find("operator") == 0) {
                replace_all(name, "+", "_add");
                replace_all(name, "-", "_sub");
                replace_all(name, "*", "_mul");
                replace_all(name, "/", "_div");
                replace_all(name, "=", "_equals");
            }
            name = "test_" + name;
            if (!i_test_interface->has_method(name))
                addMethod(i_test_interface, name);
        }
    }

    if (cls->name == "CollisionChecker") {
        std::cout << "\n";
    }
    // Если есть реальная реализация без I
    std::string test_implementation_name = i_test_interface->name.substr(1);
    if (_model->hasClass(test_implementation_name)) {
        auto impl = _model->get_class(test_implementation_name);
        for (auto &f : impl->functions) {
            if (f.name.find("test_", 0) == 0) {
                // если нет в тестовом интерфейсе
                auto it = find_if(i_test_interface->functions.begin(),
                                  i_test_interface->functions.end(),
                                  [&](auto &mf) { return mf.name == f.name; });
                if (it == i_test_interface->functions.end()) {
                    Function m;
                    m.name = f.name;
                    m.return_type = Objects::VOID;
                    //                    m.is_abstract = true;
                    i_test_interface->functions.push_back(m);
                    generateMessagesIfEmpty(impl,
                                            i_test_interface->functions.back());
                    f.is_virtual = true;
                }
            }
        }
    }

    // Генерируем execute()
    Function exec;
    exec.name = "execute";
    exec.return_type = Objects::VOID;
    for (auto &mf : i_test_interface->functions) {
        if (mf.name != "initialize" && mf.name != "execute") {
            exec.body += "this->" + mf.name + "();\n";
        }
    }
    i_test_interface->functions.push_back(std::move(exec));

    tests_.push_back(i_test_interface);
    return i_test_interface;
}

void GeneratorUnitTestsInterface::addMethod(shared_ptr<Class> &class_,
                                            const string &name) {
    if (class_->name == "ITestNullishOperator")
        std::cout << "";
    bool hasImplementation = true;
    auto subs = _model->getSubclassesOfClass(class_->name);
    for (auto &sub : subs) {
        auto m = sub->get_method(name);
        if (!m || m->is_abstract) {
            hasImplementation = false;
            Error::warning(Error::WARNING_TEST_CLASS_NOT_IMPLEMENT_METHOD,
                           class_->name, name);
            break;
        } else if (m && !m->is_virtual) {
            m->is_virtual = true;
        }
    }
    Function method;
    method.name = name;
    method.return_type = Objects::VOID;
    method.is_abstract = hasImplementation;
    method.is_virtual = true;
    class_->functions.push_back(std::move(method));
}

shared_ptr<Class> GeneratorUnitTestsInterface::generateAllTestsClass() {
    auto testAll = make_shared<Class>();
    testAll->type = "class";
    testAll->name = "RunAllTests";
    testAll->group = "tests";

    // Члены для каждого ITestX в tests_
    for (auto &t : tests_) {
        string bare = t->name.substr(1); // убрать ведущий 'I'
        if (!_model->hasClass(bare))
            continue;
        Object m;
        m.type = bare;
        m.name = getMemberName(bare);
        testAll->members.push_back(std::move(m));
    }

    // initialize(Logger*)
    Function init;
    init.name = "initialize";
    init.return_type = Objects::VOID;
    init.callable_args.push_back(Object("Logger", "logger"));
    init.callable_args.back().is_pointer = true;
    for (auto &m : testAll->members) {
        init.body += "this->" + m.name + ".initialize(logger);\n";
    }
    testAll->functions.push_back(std::move(init));

    // execute():bool
    Function exec;
    exec.name = "execute";
    exec.return_type = Objects::BOOL;
    for (auto &m : testAll->members) {
        exec.body += "this->" + m.name + ".execute();\n";
    }
    exec.body += "bool result = true;\n";
    for (auto &m : testAll->members) {
        exec.body += "result = result && this->" + m.name + ".result;\n";
    }
    exec.body += "return result;";
    testAll->functions.push_back(std::move(exec));

    return testAll;
}

void GeneratorUnitTestsInterface::generateMessagesIfEmpty(
    const shared_ptr<Class> &cls, Function &method) {
    for (auto &entry : ASSERTS) {
        const auto &assertCall = entry.first;
        int needed = entry.second;
        //        for (size_t i = 0; i < method.operations.size(); ++i) {
        auto &line = method.body;
        if (line.rfind(assertCall, 0) == 0) {
            // вырезаем аргументы между assertCall и последней ')'
            size_t start = assertCall.size();
            size_t end = line.rfind(')');
            string inside = line.substr(start, end - start);
            auto parts = smartSplit(inside, ',', {"(", ")"});
            if ((int)parts.size() <= needed) {
                string name =
                    assertCall.substr(strlen("this->"), // убрать "this->"
                                      assertCall.size() - strlen("this->") - 1);
                std::string msg = format_indexes(
                    R"("{0} is false in {1}::{2}\\nArgs:{3}")", name, cls->name,
                    method.name, join(parts, ','));
                line = line.substr(0, end) + ", " + msg + ");";
            }
        }
    }
}

string GeneratorUnitTestsInterface::getMemberName(const string &clsName) {
    string out;
    for (size_t i = 0; i < clsName.size(); ++i) {
        char c = clsName[i];
        if (isupper(c) && i > 0)
            out.push_back('_');
        out.push_back((char)tolower(c));
    }
    return out;
}
