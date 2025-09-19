#ifndef __mg_ITestFunctionTemplates_h__
#define __mg_ITestFunctionTemplates_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "TestCase.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class ITestFunctionTemplates : public TestCase
    {
    public:
        ITestFunctionTemplates();
        virtual ~ITestFunctionTemplates();
        virtual void test_add() = 0;
        virtual void test_use_other_class() = 0;
        virtual void test_add_strings() = 0;
        void execute();
        bool operator ==(const ITestFunctionTemplates& rhs) const;
        bool operator !=(const ITestFunctionTemplates& rhs) const;
        ITestFunctionTemplates(const ITestFunctionTemplates& rhs);
        const ITestFunctionTemplates& operator =(const ITestFunctionTemplates& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestFunctionTemplates_h__
