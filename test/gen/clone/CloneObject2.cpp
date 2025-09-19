#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../data/DataUnit.h"
#include "CloneObject.h"
#include "CloneObject2.h"
#include <atomic>
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string CloneObject2::TYPE("CloneObject2");

    CloneObject2::CloneObject2()
    : ptr(nullptr)
    , list1()
    , list2()
    , map1()
    , map2()
    , map3()
    , map4()
    , map5()
    , map6()
    , map7()
    , map8()
    {

    }

    CloneObject2::~CloneObject2(){}

    void CloneObject2::foo()
    {

    }

    bool CloneObject2::operator ==(const CloneObject2& rhs) const
    {

        bool result = this->CloneObject::operator ==(rhs);
        result = result && ((this->ptr == rhs.ptr) || (this->ptr != nullptr && rhs.ptr != nullptr && *this->ptr == *rhs.ptr));
        result = result && this->list1 == rhs.list1;
        result = result && this->list2 == rhs.list2;
        result = result && this->map1 == rhs.map1;
        result = result && this->map2 == rhs.map2;
        result = result && this->map3 == rhs.map3;
        result = result && this->map4 == rhs.map4;
        result = result && this->map5 == rhs.map5;
        result = result && this->map6 == rhs.map6;
        result = result && this->map7 == rhs.map7;
        result = result && this->map8 == rhs.map8;
        return result;
    }

    bool CloneObject2::operator !=(const CloneObject2& rhs) const
    {

        return !(*this == rhs);
    }

    CloneObject2::CloneObject2(const CloneObject2& rhs)
    {

        this->operator=(rhs);
    }

    const CloneObject2& CloneObject2::operator =(const CloneObject2& rhs)
    {

        this->CloneObject::operator=(rhs);
        this->ptr = rhs.ptr;
        this->list1 = rhs.list1;
        this->list2 = rhs.list2;
        this->map1 = rhs.map1;
        this->map2 = rhs.map2;
        this->map3 = rhs.map3;
        this->map4 = rhs.map4;
        this->map5 = rhs.map5;
        this->map6 = rhs.map6;
        this->map7 = rhs.map7;
        this->map8 = rhs.map8;
        return *this;
    }

    std::string CloneObject2::get_type() const
    {
        return CloneObject2::TYPE;
    }

    void CloneObject2::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(ptr, "ptr");
        serializer.serialize(list1, "list1");
        serializer.serialize(list2, "list2");
        serializer.serialize(map1, "map1");
        serializer.serialize(map2, "map2");
        serializer.serialize(map3, "map3");
        serializer.serialize(map4, "map4");
        serializer.serialize(map5, "map5");
        serializer.serialize(map6, "map6");
        serializer.serialize(map7, "map7");
        serializer.serialize(map8, "map8");
    }

    void CloneObject2::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(ptr, "ptr");
        deserializer.deserialize(list1, "list1");
        deserializer.deserialize(list2, "list2");
        deserializer.deserialize(map1, "map1");
        deserializer.deserialize(map2, "map2");
        deserializer.deserialize(map3, "map3");
        deserializer.deserialize(map4, "map4");
        deserializer.deserialize(map5, "map5");
        deserializer.deserialize(map6, "map6");
        deserializer.deserialize(map7, "map7");
        deserializer.deserialize(map8, "map8");
    }

    void CloneObject2::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(ptr, "ptr");
        serializer.serialize(list1, "list1");
        serializer.serialize(list2, "list2");
        serializer.serialize(map1, "map1");
        serializer.serialize(map2, "map2");
        serializer.serialize(map3, "map3");
        serializer.serialize(map4, "map4");
        serializer.serialize(map5, "map5");
        serializer.serialize(map6, "map6");
        serializer.serialize(map7, "map7");
        serializer.serialize(map8, "map8");
    }

    void CloneObject2::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(ptr, "ptr");
        deserializer.deserialize(list1, "list1");
        deserializer.deserialize(list2, "list2");
        deserializer.deserialize(map1, "map1");
        deserializer.deserialize(map2, "map2");
        deserializer.deserialize(map3, "map3");
        deserializer.deserialize(map4, "map4");
        deserializer.deserialize(map5, "map5");
        deserializer.deserialize(map6, "map6");
        deserializer.deserialize(map7, "map7");
        deserializer.deserialize(map8, "map8");
    }

} // namespace mg
