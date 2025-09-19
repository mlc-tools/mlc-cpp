#ifndef __mg_TestFactory_h__
#define __mg_TestFactory_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class Logger;

    class TestFactory
    {
    public:
        TestFactory();
        ~TestFactory();
        static bool tests(Logger* logger);
        static bool test(Logger* logger);
        void retain();
        int release();
        bool operator ==(const TestFactory& rhs) const;
        bool operator !=(const TestFactory& rhs) const;
        TestFactory(const TestFactory& rhs);
        const TestFactory& operator =(const TestFactory& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestFactory_h__
