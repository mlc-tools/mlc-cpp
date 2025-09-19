#ifndef __mg_TestCase_h__
#define __mg_TestCase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Logger.h"
#include <atomic>
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

    class TestCase
    {
    public:
        TestCase();
        virtual ~TestCase();
void initialize(Logger* logger);
virtual void execute() = 0;
private:
void add_result(bool result, const std::string& message);
public:
void assertTrue(bool expression, const std::string& message="");
void assertFalse(bool expression, const std::string& message="");
template <class L, class R> void assertEqual(L left, R right, const std::string& message="")
{

        this->add_result(left == right, message);
    
}
template <class L, class R> void assertNotEqual(L left, R right, const std::string& message="")
{

        this->add_result(left != right, message);
    
}
template <class T> void assertNull(T value, const std::string& message="")
{

        this->add_result(value == nullptr, message);
    
}
template <class T> void assertNotNull(T value, const std::string& message="")
{

        this->add_result(value != nullptr, message);
    
}
template <class Key, class Value> void assertInMap(Key key, const std::map<Key, Value>& map, const std::string& message="")
{

        this->add_result(in_map(key, map), message);
    
}
template <class Key, class Value> void assertNotInMap(Key key, const std::map<Key, Value>& map, const std::string& message="")
{

        this->add_result(!in_map(key, map), message);
    
}
template <class T> void assertInList(T item, const std::vector<T>& list, const std::string& message="")
{

        this->add_result(in_list(item, list), message);
    
}
template <class T> void assertNotInList(T item, const std::vector<T>& list, const std::string& message="")
{

        this->add_result(!in_list(item, list), message);
    
}
template <class T> void assertInRange(T value, T min_value, T max_value, const std::string& message="")
{

        bool result = value >= min_value && value <= max_value;
        this->add_result(result, message);
    
}
template <class T> void assertNotInRange(T value, T min_value, T max_value, const std::string& message="")
{

        bool result = value < min_value || value > max_value;
        this->add_result(result, message);
    
}
void retain();
int release();
bool operator ==(const TestCase& rhs) const;
bool operator !=(const TestCase& rhs) const;
 TestCase(const TestCase& rhs);
const TestCase& operator =(const TestCase& rhs);
virtual std::string get_type() const;
virtual void serialize_xml(SerializerXml& serializer) const;
virtual void deserialize_xml(DeserializerXml& deserializer);
virtual void serialize_json(SerializerJson& serializer) const;
virtual void deserialize_json(DeserializerJson& deserializer);

intrusive_ptr<Logger> logger;
bool result;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TestCase_h__
