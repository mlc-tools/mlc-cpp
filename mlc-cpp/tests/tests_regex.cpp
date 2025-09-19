//
//  tests_regex.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 19.09.2025.
//

#include <regex>
#include <string>
#include <iostream>
#include <cassert>
#include "../module_cpp/TranslatorCpp.hpp"

namespace tests {

std::tuple<std::regex,std::string,std::vector<std::string>> pattern_list_do = {
    std::regex(R"(list_do\(([\w\d\-\>\.\[\]]+),\s*\((\w+)\s*:>\s*(.+)\)\))"),
        R"(for(int __index__ = 0; __index__ < $1.size(); ++__index__)
{
auto& $2 = $1.at(__index__);
$3;
})",
    { "list_do" }
};

void test_regex_list_do()
{
    std::string text = "list_do(this->test_list_lambda2, (model :> model->data = DataStorage::shared().get<DataUnit>(name)));";
    std::string wait_result = R"(for(int __index__ = 0; __index__ < this->test_list_lambda2.size(); ++__index__)
{
auto& model = this->test_list_lambda2.at(__index__);
model->data = DataStorage::shared().get<DataUnit>(name);
};)";
    
    auto result = std::regex_replace(text, std::get<0>(pattern_list_do), std::get<1>(pattern_list_do));
    assert(result == wait_result);
}

void test_replace_list_do_with_translator(){
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
    auto result = translator.replacePattern(text, pattern_list_do);
    assert(result == wait_result);
}

void run_regex_tests(){
    test_regex_list_do();
    test_replace_list_do_with_translator();
}

}
