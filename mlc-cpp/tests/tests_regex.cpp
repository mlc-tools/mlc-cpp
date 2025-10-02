//
//  tests_regex.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 19.09.2025.
//

#include "../module_cpp/RegexPatternCpp.hpp"
#include "../module_cpp/TranslatorCpp.hpp"
#include "Error.hpp"
#include <cassert>
#include <iostream>
#include <re2/re2.h>
#include <string>

namespace tests {

RegexPattern pattern_list_do = {
    std::make_unique<RE2>(
        R"(list_do\(([\w\d\-\>\.\[\]]+),\s*\((\w+)\s*:>\s*(.+)\)\))"),
    R"(for(int __index__ = 0; __index__ < \1.size(); ++__index__)
{
auto& \2 = \1.at(__index__);
\3;
})",
    {"list_do"}};

void test_regex_list_do() {
    static re2::RE2 re2(
        R"(list_do\(([\w\d\-\>\.\[\]]+),\s*\((\w+)\s*:>\s*(.+)\)\))");
    static std::string replace =
        R"(for(int __index__ = 0; __index__ < \1.size(); ++__index__)
{
auto& \2 = \1.at(__index__);
\3;
})";

    std::string text = "list_do(this->test_list_lambda2, (model :> model->data "
                       "= DataStorage::shared().get<DataUnit>(name)));";
    std::string wait_result =
        R"(for(int __index__ = 0; __index__ < this->test_list_lambda2.size(); ++__index__)
{
auto& model = this->test_list_lambda2.at(__index__);
model->data = DataStorage::shared().get<DataUnit>(name);
};)";

    RE2::GlobalReplace(&text, re2, replace);
    assert(text == wait_result);
}

void test_replace_list_do_with_translator() {
    TranslatorCpp translator;
    std::string text = R"__(
        list_clear(this->test_list_lambda2);
        list_push(this->test_list_lambda2, new Model());
        list_push(this->test_list_lambda2, new Model());
        std::string name = @{{__string_0__}};
        list_do(this->test_list_lambda2, (model :> model->data = DataStorage::shared().get<DataUnit>(name)));

        this->assertEqual(this->test_list_lambda2[0]->data->name, name);
        this->assertEqual(this->test_list_lambda2[1]->data->name, name);
    )__";
    std::string wait_result = R"__(
        list_clear(this->test_list_lambda2);
        list_push(this->test_list_lambda2, new Model());
        list_push(this->test_list_lambda2, new Model());
        std::string name = @{{__string_0__}};
        for(int __index__ = 0; __index__ < this->test_list_lambda2.size(); ++__index__)
{
auto& model = this->test_list_lambda2.at(__index__);
model->data = DataStorage::shared().get<DataUnit>(name);
};

        this->assertEqual(this->test_list_lambda2[0]->data->name, name);
        this->assertEqual(this->test_list_lambda2[1]->data->name, name);
    )__";
    translator.replacePattern(text, pattern_list_do);
    assert(text == wait_result);
}

void test_make_intrusive() {
    std::string text = "auto movement = new MoveToTarget();";
    std::string wait = "auto movement = make_intrusive<MoveToTarget>();";
    TranslatorCpp translator;
    for (auto &pattern : RegexPatternCpp::functionPatterns) {
        translator.replacePattern(text, pattern);
    }
    assert(text == wait);
}

void run_regex_tests() {
    test_regex_list_do();
    test_replace_list_do_with_translator();
    test_make_intrusive();
}

} // namespace tests
