#ifndef __mg_TestToFor_h__
#define __mg_TestToFor_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "TestEnum.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestToFor : public TestEnum
    {
    public:
        TestToFor();
        ~TestToFor();
        bool operator ==(const TestToFor& rhs) const;
        bool operator !=(const TestToFor& rhs) const;
        TestToFor(const TestToFor& rhs);
        const TestToFor& operator =(const TestToFor& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

        int some_count;
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestToFor_h__
