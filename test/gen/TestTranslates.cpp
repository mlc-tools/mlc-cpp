#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "DataStorage.h"
#include "Point.h"
#include "Request.h"
#include "RequestBar.h"
#include "TestTranslates.h"
#include "TestTranslatesModel.h"
#include "Translates.h"
#include "data/DataUnit.h"
#include "tests/ITestTranslates.h"
#include <map>
#include <string>
#include <vector>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestTranslates::TYPE("TestTranslates");

    TestTranslates::TestTranslates()
    : test_map_models()
    , test_list_lambda()
    , test_list_lambda2()
    , test_list_swap1()
    , test_list_swap2()
    {

    }

    TestTranslates::~TestTranslates(){}

    void TestTranslates::test_map_remove_if()
    {

        map_clear(this->test_map_models);
        this->test_map_models["1"] = make_intrusive<TestTranslatesModel>();
        this->test_map_models["1"]->data = DataStorage::shared().get<DataUnit>("unitname1");
        this->test_map_models["2"] = make_intrusive<TestTranslatesModel>();
        this->test_map_models["2"]->data = nullptr;

        for(auto __iter__ = this->test_map_models.begin(); __iter__ != this->test_map_models.end();)
        {
            auto& key = __iter__->first; auto& value = __iter__->second; (void)key;(void)value;
            if(value->data == nullptr) __iter__ = this->test_map_models.erase(__iter__);
            else ++__iter__;
        };

        this->assertEqual(map_size(this->test_map_models), 1);
        this->assertTrue(in_map("1", this->test_map_models));

    }

    void TestTranslates::test_map_do_if()
    {

        map_clear(this->test_map_models);
        this->test_map_models["1"] = make_intrusive<TestTranslatesModel>();
        this->test_map_models["1"]->data = DataStorage::shared().get<DataUnit>("unitname1");
        this->test_map_models["2"] = make_intrusive<TestTranslatesModel>();
        this->test_map_models["2"]->data = nullptr;

        for(auto __iter__ = this->test_map_models.begin(); __iter__ != this->test_map_models.end();)
        {
            auto& key = __iter__->first; auto& value = __iter__->second; (void)key;(void)value;
            if(value->data == nullptr ) { ++__iter__; value->data = DataStorage::shared().get<DataUnit>("unitname1"); }
            else ++__iter__;
        }

        this->assertEqual(map_size(this->test_map_models), 2);
        this->assertTrue(in_map("1", this->test_map_models));
        this->assertTrue(in_map("2", this->test_map_models));
        this->assertTrue(this->test_map_models["2"]->data != nullptr);

        this->test_map_models["2"]->data = nullptr;
        for(auto __iter__ = this->test_map_models.begin(); __iter__ != this->test_map_models.end();)
        {
            auto& key = __iter__->first; auto& value = __iter__->second; (void)key;(void)value;
            if(value->data == nullptr ) { ++__iter__; map_remove(this->test_map_models, key); }
            else ++__iter__;
        }

        this->assertEqual(map_size(this->test_map_models), 1);
        this->assertTrue(in_map("1", this->test_map_models));
        this->assertFalse(in_map("2", this->test_map_models));

    }

    void TestTranslates::test_list_remove_if()
    {

        list_clear(this->test_list_lambda);
        list_push(this->test_list_lambda, 3);
        list_push(this->test_list_lambda, 4);
        auto iter = std::remove_if(this->test_list_lambda.begin(), this->test_list_lambda.end(), [&](const auto& val)
        {
            return val == 3;
        });
        this->test_list_lambda.erase(iter, this->test_list_lambda.end());;

        this->assertEqual(list_size(this->test_list_lambda), 1);
        this->assertEqual(this->test_list_lambda[0], 4);

    }

    void TestTranslates::test_list_do_if()
    {

        list_clear(this->test_list_lambda);
        list_push(this->test_list_lambda, 3);
        list_push(this->test_list_lambda, 4);
        for(int __index__ = 0; __index__ < this->test_list_lambda.size(); ++__index__)
        {
            auto& value = this->test_list_lambda.at(__index__);
            if(value == 3 )
            {
                auto __size__ = this->test_list_lambda.size();
                list_remove(this->test_list_lambda, 3);
                if(__size__ != this->test_list_lambda.size()) { --__index__; }
            }
        }

        this->assertEqual(list_size(this->test_list_lambda), 1);
        this->assertEqual(this->test_list_lambda[0], 4);

    }

    void TestTranslates::test_list_do()
    {

        list_clear(this->test_list_lambda2);
        list_push(this->test_list_lambda2, make_intrusive<TestTranslatesModel>());
        list_push(this->test_list_lambda2, make_intrusive<TestTranslatesModel>());
        std::string name = "unitname1";
        for(int __index__ = 0; __index__ < this->test_list_lambda2.size(); ++__index__)
        {
            auto& model = this->test_list_lambda2.at(__index__);
            model->data = DataStorage::shared().get<DataUnit>(name);
        };

        this->assertEqual(this->test_list_lambda2[0]->data->name, name);
        this->assertEqual(this->test_list_lambda2[1]->data->name, name);

    }

    void TestTranslates::test_dummy_function()
    {

    }

    void TestTranslates::test_strcat()
    {

        std::string a = "a";
        std::string b = "b";
        std::string c = (std::string(a) + std::string(b));
        this->assertTrue(c == "ab", "std::strcat correct");

    }

    void TestTranslates::test_strcat2()
    {

        std::string c = (std::string("a") + std::string("b"));
        this->assertTrue(c == "ab", "std::strcat correct");

    }

    void TestTranslates::test_create_objects()
    {

        auto object = make_intrusive<Translates>();
        this->assertTrue(object != nullptr, "Create object by new");

    }

    void TestTranslates::test_list()
    {

        std::vector<int> l0;
        std::vector<int*> l1;
        std::vector<std::string> l2;
        std::vector<intrusive_ptr<Translates>> l3;

        list_push(l0, 1);
        list_push(l1, nullptr);
        list_push(l2, "");
        list_push(l3, make_intrusive<Translates>());

        this->assertTrue(in_list(1, l0));
        this->assertTrue(in_list(nullptr, l1));
        this->assertTrue(in_list("", l2));
        this->assertTrue(list_size(l3) == 1);

    }

    void TestTranslates::test_list_index()
    {

        std::vector<int> l;
        list_push(l, 1);
        list_push(l, 2);
        list_push(l, 5);
        list_push(l, 4);
        list_push(l, 3);

        this->assertTrue(list_index(l, 1) == 0);
        this->assertTrue(list_index(l, 2) == 1);
        this->assertTrue(list_index(l, 5) == 2);
        this->assertTrue(list_index(l, 4) == 3);
        this->assertTrue(list_index(l, 3) == 4);
        this->assertTrue(list_index(l, 0) == -1);

    }

    void TestTranslates::test_map()
    {

        std::map<int, int> m0;
        std::map<int, int*> m1;
        std::map<std::string, std::string> m2;
        std::map<std::string, Translates*> m3;

        m0[0] = 1;
        m1[0] = nullptr;
        m2[""] = "";
        m3[""] = make_intrusive<Translates>();

        this->assertTrue(in_map(0, m0));
        this->assertTrue(m0[0] == 1);

        this->assertTrue(in_map(0, m1));
        this->assertTrue(m1[0] == nullptr);

        this->assertTrue(in_map("", m2));
        this->assertTrue(m2[""] == "");

        this->assertTrue(map_size(m3) == 1);

    }

    void TestTranslates::test_to_str()
    {

        int a = 1;
        std::string s = "1";
        this->assertTrue(toStr(a) == s);

    }

    void TestTranslates::test_str_to()
    {

        std::string s = "1";
        this->assertTrue(strTo<int>(s) == 1);
        this->assertTrue(strTo<float>(s) == 1.f);
        this->assertTrue(strTo<bool>("yes") == true);
        this->assertTrue(strTo<bool>("true") == true);
        this->assertTrue(strTo<bool>("no") == false);
        this->assertTrue(strTo<bool>("false") == false);
        this->assertTrue(strTo<bool>("") == false);

    }

    void TestTranslates::test_at()
    {

        std::vector<int> vector;
        list_push(vector, 1);
        this->assertTrue(vector.at(0) == 1);

        std::map<int, int> map;
        map[0] = 1;
        this->assertTrue(map.at(0) == 1);

    }

    void TestTranslates::test_list_remove()
    {

        std::vector<int> vector;
        list_push(vector, 1);
        list_push(vector, 2);
        list_push(vector, 3);
        list_push(vector, 4);

        list_remove(vector, 2);
        this->assertTrue(list_size(vector) == 3);
        this->assertTrue(vector[0] == 1);
        this->assertTrue(vector[1] == 3);
        this->assertTrue(vector[2] == 4);

    }

    void TestTranslates::test_list_clear()
    {

        std::vector<int> vector;
        list_push(vector, 1);
        list_push(vector, 2);
        list_push(vector, 3);
        list_push(vector, 4);

        list_clear(vector);
        this->assertTrue(list_size(vector) == 0);

    }

    void TestTranslates::test_list_resize()
    {

        std::vector<int> vector;

        list_resize(vector, 1);
        this->assertTrue(list_size(vector) == 1);
        list_resize(vector, 2);
        this->assertTrue(list_size(vector) == 2);
        list_resize(vector, 10);
        this->assertTrue(list_size(vector) == 10);

        vector[0] = 123;
        list_resize(vector, 1);
        this->assertTrue(list_size(vector) == 1);
        this->assertTrue(vector[0] == 123);

    }

    void TestTranslates::test_string_size()
    {

        this->assertTrue(string_size("") == 0);
        this->assertTrue(string_size("s") == 1);
        this->assertTrue(string_size("1") == 1);
        this->assertTrue(string_size("00") == 2);

        std::string s = "123";
        this->assertTrue(string_size(s) == 3);

    }

    void TestTranslates::test_string_empty()
    {

        this->assertTrue(string_empty(""));
        this->assertFalse(string_empty("s"));
        this->assertFalse(string_empty("1"));
        this->assertFalse(string_empty("."));

    }

    void TestTranslates::test_map_clear()
    {

        std::map<int, int> map;
        map[1] = 2;
        map[2] = 3;
        map_clear(map);
        this->assertEqual(map_size(map), 0, "Map is empty");

    }

    void TestTranslates::test_map_remove()
    {

        std::map<int, int> map;
        map[1] = 2;
        map[2] = 3;
        map_remove(map, 1);

        this->assertTrue(in_map(2, map));
        this->assertFalse(in_map(1, map));

    }

    void TestTranslates::test_list_push()
    {

        std::vector<Point> result;
        Point point;
        point.x = 1;
        point.y = 2;
        list_push(result, Point::create(point.x, point.y-1));

    }

    void TestTranslates::test_list_insert()
    {

        std::vector<int> vector;

        list_insert(vector, 0, 1);
        this->assertTrue(list_size(vector) == 1);
        this->assertTrue(vector[0] == 1);

        list_insert(vector, 0, 2);
        this->assertTrue(list_size(vector) == 2);
        this->assertTrue(vector[0] == 2);
        this->assertTrue(vector[1] == 1);

        list_insert(vector, 1, 3);
        this->assertTrue(list_size(vector) == 3);
        this->assertTrue(vector[0] == 2);
        this->assertTrue(vector[1] == 3);
        this->assertTrue(vector[2] == 1);

    }

    void TestTranslates::test_list_erase()
    {

        std::vector<int> vector;
        list_push(vector, 1);
        list_push(vector, 2);
        list_push(vector, 3);
        list_push(vector, 4);

        list_erase(vector, 0);
        this->assertTrue(list_size(vector) == 3);
        this->assertTrue(vector[0] == 2);
        this->assertTrue(vector[1] == 3);
        this->assertTrue(vector[2] == 4);

        list_erase(vector, 1);
        this->assertTrue(list_size(vector) == 2);
        this->assertTrue(vector[0] == 2);
        this->assertTrue(vector[1] == 4);

        list_erase(vector, 1);
        this->assertTrue(list_size(vector) == 1);
        this->assertTrue(vector[0] == 2);

    }

    void TestTranslates::test_list_truncate()
    {

        std::vector<int> vector;
        list_push(vector, 1);
        list_push(vector, 2);
        list_push(vector, 3);
        list_push(vector, 4);

        list_truncate(vector, 3);
        this->assertTrue(list_size(vector) == 3);
        this->assertTrue(vector[0] == 1);
        this->assertTrue(vector[1] == 2);
        this->assertTrue(vector[2] == 3);

        list_truncate(vector, 1);
        this->assertTrue(list_size(vector) == 1);
        this->assertTrue(vector[0] == 1);

        list_truncate(vector, 0);
        this->assertTrue(list_size(vector) == 0);

    }

    void TestTranslates::test_translate_declare_empty_string()
    {

        std::map<int, std::string> possible_objects;
        possible_objects[1] = "1";
        std::vector<int> indexes;
        list_push(indexes, 1);

        for(auto index : indexes)
        {
            std::string object_name;
            for (auto&& pair : possible_objects)
            {
                auto& key = pair.first;
                auto& value = pair.second;
                (void)key; // don't generate 'Unused variable' warning
                (void)value; // don't generate 'Unused variable' warning
                object_name = key;
                break;
            }
        }

    }

    void TestTranslates::test_translate_for_with_size_t()
    {

        size_t count = 1;
        for(size_t i=0; i<count; ++i)
        {
            i += 1;
        }

    }

    void TestTranslates::test_translate_dynamic_pointer_cast_intrusive()
    {

        auto request = make_intrusive<RequestBar>();

        auto request_base = request;
        this->assertTrue(request_base != nullptr);
        this->assertEqual(request_base->get_type(), RequestBar::TYPE, "");

        auto request_bar = dynamic_pointer_cast_intrusive<RequestBar>(request_base);
        this->assertTrue(request_bar != nullptr);
        this->assertEqual(request_bar->get_type(), RequestBar::TYPE, "");

        auto request_bar2 = dynamic_pointer_cast_intrusive<RequestBar, Request>(request_base);
        this->assertTrue(request_bar2 != nullptr);
        this->assertEqual(request_bar2->get_type(), RequestBar::TYPE, "");

    }

    void TestTranslates::test_min_max()
    {

        this->assertEqual(std::min(1, 2), 1);
        this->assertEqual(std::max(1, 2), 2);
        this->assertEqual(std::min<int>(1, 2), 1);
        this->assertEqual(std::max<int>(1, 2), 2);
        this->assertEqual(std::min<float>(1, 2), 1.f);
        this->assertEqual(std::max<float>(1, 2), 2.f);

    }

    void TestTranslates::test_swap()
    {

        list_push(this->test_list_swap1, 1);
        list_push(this->test_list_swap1, 2);
        list_push(this->test_list_swap2, 3);
        mg_swap(this->test_list_swap1, this->test_list_swap2);
        this->assertEqual(list_size(this->test_list_swap1), 1);
        this->assertEqual(list_size(this->test_list_swap2), 2);
        this->assertEqual(this->test_list_swap1[0], 3);
        this->assertEqual(this->test_list_swap2[0], 1);
        this->assertEqual(this->test_list_swap2[1], 2);

        int a = 1;
        int b = 2;
        mg_swap(a, b);
        this->assertEqual(a, 2);
        this->assertEqual(b, 1);

        std::string sa = "1";
        std::string sb = "2";
        mg_swap(sa, sb);
        this->assertTrue(sa == "2");
        this->assertTrue(sb == "1");

    }

    void TestTranslates::test_split()
    {

        auto string = "123x4568";
        auto values = split(string, 'x');
        this->assertEqual(list_size(values), 2);
        this->assertTrue(values[0] == "123");
        this->assertTrue(values[1] == "4568");
        this->assertTrue(join(values, 'x') == string);

        string = "123x";
        values = split(string, 'x');
        this->assertEqual(list_size(values), 2);
        this->assertTrue(values[0] == "123");
        this->assertTrue(values[1] == "");
        this->assertTrue(join(values, 'x') == string);

        string = "x123";
        values = split(string, 'x');
        this->assertEqual(list_size(values), 2);
        this->assertTrue(values[0] == "");
        this->assertTrue(values[1] == "123");
        this->assertTrue(join(values, 'x') == string);

        string = "x";
        values = split(string, 'x');
        this->assertEqual(list_size(values), 2);
        this->assertTrue(values[0] == "");
        this->assertTrue(values[1] == "");
        this->assertTrue(join(values, 'x') == string);

        string = "";
        values = split(string, 'x');
        this->assertEqual(list_size(values), 1);
        this->assertTrue(join(values, 'x') == string);

        string = "1,,2,,3";
        values = split(string, ',');
        this->assertEqual(list_size(values), 5);
        this->assertTrue(values[0] == "1");
        this->assertTrue(values[1] == "");
        this->assertTrue(values[2] == "2");
        this->assertTrue(values[3] == "");
        this->assertTrue(values[4] == "3");
        this->assertTrue(join(values, ',') == string);

    }

    bool TestTranslates::operator ==(const TestTranslates& rhs) const
    {

        bool result = this->ITestTranslates::operator ==(rhs);
        result = result && this->test_map_models == rhs.test_map_models;
        result = result && this->test_list_lambda == rhs.test_list_lambda;
        result = result && this->test_list_lambda2 == rhs.test_list_lambda2;
        result = result && this->test_list_swap1 == rhs.test_list_swap1;
        result = result && this->test_list_swap2 == rhs.test_list_swap2;
        return result;
    }

    bool TestTranslates::operator !=(const TestTranslates& rhs) const
    {

        return !(*this == rhs);
    }

    TestTranslates::TestTranslates(const TestTranslates& rhs)
    {

        this->operator=(rhs);
    }

    const TestTranslates& TestTranslates::operator =(const TestTranslates& rhs)
    {

        this->ITestTranslates::operator=(rhs);
        this->test_map_models = rhs.test_map_models;
        this->test_list_lambda = rhs.test_list_lambda;
        this->test_list_lambda2 = rhs.test_list_lambda2;
        this->test_list_swap1 = rhs.test_list_swap1;
        this->test_list_swap2 = rhs.test_list_swap2;
        return *this;
    }

    std::string TestTranslates::get_type() const
    {
        return TestTranslates::TYPE;
    }

    void TestTranslates::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(test_map_models, "test_map_models");
        serializer.serialize(test_list_lambda, "test_list_lambda");
        serializer.serialize(test_list_lambda2, "test_list_lambda2");
        serializer.serialize(test_list_swap1, "test_list_swap1");
        serializer.serialize(test_list_swap2, "test_list_swap2");
    }

    void TestTranslates::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(test_map_models, "test_map_models");
        deserializer.deserialize(test_list_lambda, "test_list_lambda");
        deserializer.deserialize(test_list_lambda2, "test_list_lambda2");
        deserializer.deserialize(test_list_swap1, "test_list_swap1");
        deserializer.deserialize(test_list_swap2, "test_list_swap2");
    }

    void TestTranslates::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(test_map_models, "test_map_models");
        serializer.serialize(test_list_lambda, "test_list_lambda");
        serializer.serialize(test_list_lambda2, "test_list_lambda2");
        serializer.serialize(test_list_swap1, "test_list_swap1");
        serializer.serialize(test_list_swap2, "test_list_swap2");
    }

    void TestTranslates::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(test_map_models, "test_map_models");
        deserializer.deserialize(test_list_lambda, "test_list_lambda");
        deserializer.deserialize(test_list_lambda2, "test_list_lambda2");
        deserializer.deserialize(test_list_swap1, "test_list_swap1");
        deserializer.deserialize(test_list_swap2, "test_list_swap2");
    }

} // namespace mg
