#ifndef __mg_BarClassFromBase_h__
#define __mg_BarClassFromBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "BaseClass.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class BarClassFromBase : public BaseClass
    {
    public:
        BarClassFromBase();
        ~BarClassFromBase();
bool operator ==(const BarClassFromBase& rhs) const;
bool operator !=(const BarClassFromBase& rhs) const;
 BarClassFromBase(const BarClassFromBase& rhs);
const BarClassFromBase& operator =(const BarClassFromBase& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_BarClassFromBase_h__
