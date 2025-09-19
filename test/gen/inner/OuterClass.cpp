#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../OuterClassInnerClass.h"
#include "OuterClass.h"
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string OuterClass::TYPE("OuterClass");

    OuterClass::OuterClass()
    : inner()
    , test_list()
    , test_map()
    , _reference_counter(1)
    {

    }

    OuterClass::~OuterClass(){}

    OuterClassInnerClass OuterClass::func()
    {

        OuterClassInnerClass inner;
        return inner;

    }

    void OuterClass::func2(OuterClassInnerClass arg)
    {

        arg.func();

    }

    void OuterClass::retain()
    {
        ++this->_reference_counter;
    }

    int OuterClass::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool OuterClass::operator ==(const OuterClass& rhs) const
    {
        bool result = true;
        result = result && this->inner == rhs.inner;
        result = result && this->test_list == rhs.test_list;
        result = result && this->test_map == rhs.test_map;
        return result;
    }

    bool OuterClass::operator !=(const OuterClass& rhs) const
    {

        return !(*this == rhs);
    }

    OuterClass::OuterClass(const OuterClass& rhs)
    {

        this->operator=(rhs);
    }

    const OuterClass& OuterClass::operator =(const OuterClass& rhs)
    {

        this->inner = rhs.inner;
        this->test_list = rhs.test_list;
        this->test_map = rhs.test_map;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string OuterClass::get_type() const
    {
        return OuterClass::TYPE;
    }

    void OuterClass::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(inner, "inner");
        serializer.serialize(test_list, "test_list");
        serializer.serialize(test_map, "test_map");
    }

    void OuterClass::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(inner, "inner");
        deserializer.deserialize(test_list, "test_list");
        deserializer.deserialize(test_map, "test_map");
    }

    void OuterClass::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(inner, "inner");
        serializer.serialize(test_list, "test_list");
        serializer.serialize(test_map, "test_map");
    }

    void OuterClass::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(inner, "inner");
        deserializer.deserialize(test_list, "test_list");
        deserializer.deserialize(test_map, "test_map");
    }

} // namespace mg
