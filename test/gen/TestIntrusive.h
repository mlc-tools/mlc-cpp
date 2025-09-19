#ifndef __mg_TestIntrusive_h__
#define __mg_TestIntrusive_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Intrusive.h"
#include "tests/ITestIntrusive.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestIntrusive : public ITestIntrusive
    {
    public:
        TestIntrusive();
        ~TestIntrusive();
        void test_add_self_to_list();
        void test_remove_self_from_list();
        bool operator ==(const TestIntrusive& rhs) const;
        bool operator !=(const TestIntrusive& rhs) const;
        TestIntrusive(const TestIntrusive& rhs);
        const TestIntrusive& operator =(const TestIntrusive& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::vector<intrusive_ptr<Intrusive>> pointers;
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestIntrusive_h__
