#ifndef __mg_TemplateMethod_h__
#define __mg_TemplateMethod_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "BaseClass.h"
#include <map>
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class TemplateMethod
    {
    public:
        TemplateMethod();
        ~TemplateMethod();
template <class T> intrusive_ptr<T> get(const std::string& name)
{

        for (auto&& pair : this->members)
{
    auto& member_name = pair.first;
    auto& member = pair.second;
    (void)member_name; // don't generate 'Unused variable' warning
    (void)member; // don't generate 'Unused variable' warning
            if(member_name == name)
            {
                return dynamic_pointer_cast_intrusive<T>(member);
            }
        }
        return nullptr;
    
}
void retain();
int release();
bool operator ==(const TemplateMethod& rhs) const;
bool operator !=(const TemplateMethod& rhs) const;
 TemplateMethod(const TemplateMethod& rhs);
const TemplateMethod& operator =(const TemplateMethod& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

std::map<std::string, intrusive_ptr<BaseClass>> members;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_TemplateMethod_h__
