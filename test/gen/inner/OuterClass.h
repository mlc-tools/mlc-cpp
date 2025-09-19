#ifndef __mg_OuterClass_h__
#define __mg_OuterClass_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../OuterClassInnerClass.h"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class OuterClassInnerClass;

    class OuterClass
    {
    public:
        OuterClass();
        ~OuterClass();
        OuterClassInnerClass func();
        void func2(OuterClassInnerClass arg);
        void retain();
        int release();
        bool operator ==(const OuterClass& rhs) const;
        bool operator !=(const OuterClass& rhs) const;
        OuterClass(const OuterClass& rhs);
        const OuterClass& operator =(const OuterClass& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        OuterClassInnerClass inner;
        std::vector<OuterClassInnerClass> test_list;
        std::map<int, OuterClassInnerClass> test_map;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_OuterClass_h__
