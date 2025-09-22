#ifndef __mg_ComponentBase_h__
#define __mg_ComponentBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentBase
    {
    public:
        ComponentBase();
        virtual ~ComponentBase();
        void retain();
        int release();
        bool operator ==(const ComponentBase& rhs) const;
        bool operator !=(const ComponentBase& rhs) const;
        ComponentBase(const ComponentBase& rhs);
        const ComponentBase& operator =(const ComponentBase& rhs);
        virtual std::string get_type() const;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) = 0;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) = 0;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

        int id;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentBase_h__
