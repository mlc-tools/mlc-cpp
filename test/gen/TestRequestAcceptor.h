#ifndef __mg_TestRequestAcceptor_h__
#define __mg_TestRequestAcceptor_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Request.h"
#include "tests/ITestRequestAcceptor.h"
#include <atomic>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class TestRequestAcceptor : public ITestRequestAcceptor
    {
    public:
        TestRequestAcceptor();
        ~TestRequestAcceptor();
        void test_visitor();
        bool operator ==(const TestRequestAcceptor& rhs) const;
        bool operator !=(const TestRequestAcceptor& rhs) const;
        TestRequestAcceptor(const TestRequestAcceptor& rhs);
        const TestRequestAcceptor& operator =(const TestRequestAcceptor& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        intrusive_ptr<Request> request;
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestRequestAcceptor_h__
