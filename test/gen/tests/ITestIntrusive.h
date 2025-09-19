#ifndef __mg_ITestIntrusive_h__
#define __mg_ITestIntrusive_h__

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

    class ITestIntrusive : public TestCase
    {
    public:
        ITestIntrusive();
        virtual ~ITestIntrusive();
        virtual void test_add_self_to_list() = 0;
        virtual void test_remove_self_from_list() = 0;
        void execute();
        bool operator ==(const ITestIntrusive& rhs) const;
        bool operator !=(const ITestIntrusive& rhs) const;
        ITestIntrusive(const ITestIntrusive& rhs);
        const ITestIntrusive& operator =(const ITestIntrusive& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ITestIntrusive_h__
