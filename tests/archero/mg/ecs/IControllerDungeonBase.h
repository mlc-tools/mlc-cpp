#ifndef __mg_IControllerDungeonBase_h__
#define __mg_IControllerDungeonBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class IControllerDungeonBase
    {
    public:
        IControllerDungeonBase();
        virtual ~IControllerDungeonBase();
        virtual bool operator ==(const IControllerDungeonBase& rhs) const;
        virtual bool operator !=(const IControllerDungeonBase& rhs) const;
        IControllerDungeonBase(const IControllerDungeonBase& rhs);
        virtual const IControllerDungeonBase& operator =(const IControllerDungeonBase& rhs);
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);


    };
} //namespace mg

#endif //#ifndef __mg_IControllerDungeonBase_h__
