#ifndef __mg_ModelBase_h__
#define __mg_ModelBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"



namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class ModelBase
    {
    public:
        ModelBase();
        ~ModelBase();
bool operator ==(const ModelBase& rhs) const;
bool operator !=(const ModelBase& rhs) const;
 ModelBase(const ModelBase& rhs);
const ModelBase& operator =(const ModelBase& rhs);
virtual void serialize_xml(SerializerXml& serializer) const;
virtual void deserialize_xml(DeserializerXml& deserializer);
virtual void serialize_json(SerializerJson& serializer) const;
virtual void deserialize_json(DeserializerJson& deserializer);


    };
} // namespace mg

#endif // __mg_ModelBase_h__
