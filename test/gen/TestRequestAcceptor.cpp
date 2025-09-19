#include "intrusive_ptr.h"
#include "mg_Factory.h"
#include "Request.h"
#include "RequestAcceptor.h"
#include "RequestBar.h"
#include "RequestBar2.h"
#include "RequestFoo.h"
#include "TestRequestAcceptor.h"
#include "tests/ITestRequestAcceptor.h"
#include <atomic>
#include <string>
#include "mg_extensions.h"
#include "SerializerJson.h"
#include "SerializerXml.h"

namespace mg
{
    const std::string TestRequestAcceptor::TYPE("TestRequestAcceptor");

    TestRequestAcceptor::TestRequestAcceptor()
    : request(nullptr)
    {

    }

    TestRequestAcceptor::~TestRequestAcceptor(){}

    void TestRequestAcceptor::test_visitor()
    {

        RequestAcceptor acceptor;

        this->request = make_intrusive<RequestFoo>();
        this->request->accept(&acceptor);
        this->assertTrue(acceptor.foo == 1);
        this->assertTrue(acceptor.bar == 0);
        this->assertTrue(acceptor.bar2 == 0);
        this->assertTrue(acceptor.base == 0);
        acceptor.foo = 0;

        this->request = make_intrusive<RequestBar>();
        this->request->accept(&acceptor);
        this->assertTrue(acceptor.foo == 0);
        this->assertTrue(acceptor.bar == 1);
        this->assertTrue(acceptor.bar2 == 0);
        this->assertTrue(acceptor.base == 0);
        acceptor.bar = 0;

        this->request = make_intrusive<RequestBar2>();
        this->request->accept(&acceptor);
        this->assertTrue(acceptor.foo == 0);
        this->assertTrue(acceptor.bar == 0);
        this->assertTrue(acceptor.bar2 == 1);
        this->assertTrue(acceptor.base == 0);
        acceptor.bar2 = 0;

    }

    bool TestRequestAcceptor::operator ==(const TestRequestAcceptor& rhs) const
    {

        bool result = this->ITestRequestAcceptor::operator ==(rhs);
        result = result && ((this->request == rhs.request) || (this->request != nullptr && rhs.request != nullptr && *this->request == *rhs.request));
        return result;
    }

    bool TestRequestAcceptor::operator !=(const TestRequestAcceptor& rhs) const
    {

        return !(*this == rhs);
    }

    TestRequestAcceptor::TestRequestAcceptor(const TestRequestAcceptor& rhs)
    {

        this->operator=(rhs);
    }

    const TestRequestAcceptor& TestRequestAcceptor::operator =(const TestRequestAcceptor& rhs)
    {

        this->ITestRequestAcceptor::operator=(rhs);
        this->request = rhs.request;
        return *this;
    }

    std::string TestRequestAcceptor::get_type() const
    {
        return TestRequestAcceptor::TYPE;
    }

    void TestRequestAcceptor::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(request, "request");
    }

    void TestRequestAcceptor::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(request, "request");
    }

    void TestRequestAcceptor::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(request, "request");
    }

    void TestRequestAcceptor::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(request, "request");
    }

} // namespace mg
