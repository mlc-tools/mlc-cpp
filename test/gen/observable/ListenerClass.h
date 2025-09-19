#ifndef __mg_ListenerClass_h__
#define __mg_ListenerClass_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "TestObservableClass.h"
#include <atomic>
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
class DataUnit;

    class ListenerClass
    {
    public:
        ListenerClass();
        ~ListenerClass();
void listen();
void listen_int(int value);
void listen_int_2(int a, int b);
void listen_link(const DataUnit* data);
void listen_pointer(DataUnit* data);
void retain();
int release();
bool operator ==(const ListenerClass& rhs) const;
bool operator !=(const ListenerClass& rhs) const;
 ListenerClass(const ListenerClass& rhs);
const ListenerClass& operator =(const ListenerClass& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

intrusive_ptr<TestObservableClass> test;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ListenerClass_h__
