#ifndef __mg_ComponentVampire_h__
#define __mg_ComponentVampire_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentVampire : public ComponentBase
    {
    public:
        ComponentVampire();
        virtual ~ComponentVampire();
        bool operator ==(const ComponentVampire& rhs) const;
        bool operator !=(const ComponentVampire& rhs) const;
        ComponentVampire(const ComponentVampire& rhs);
        const ComponentVampire& operator =(const ComponentVampire& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        float chance;
        float heal_percent;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentVampire_h__
