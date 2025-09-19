#ifndef __mg_FunctionTest_h__
#define __mg_FunctionTest_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "data/DataUnit.h"
#include <map>
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
class Logger;
class TestUser1;

    class FunctionTest
    {
    public:
        FunctionTest();
        ~FunctionTest();
void initialize();
static bool tests(Logger* logger);
static bool test_create_objects(Logger* logger);
static bool test_for(Logger* logger);
static bool test_while(Logger* logger);
static bool test_if_(Logger* logger);
static bool std_functions(Logger* logger);
static bool test_1(Logger* logger);
static bool test_1_1(TestUser1* user);
void retain();
int release();
bool operator ==(const FunctionTest& rhs) const;
bool operator !=(const FunctionTest& rhs) const;
 FunctionTest(const FunctionTest& rhs);
const FunctionTest& operator =(const FunctionTest& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

bool initialized;
const DataUnit* data_unit;
std::vector<int> list;
std::map<int, int> map;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_FunctionTest_h__
