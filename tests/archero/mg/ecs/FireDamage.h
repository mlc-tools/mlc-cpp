#ifndef __mg_FireDamage_h__
#define __mg_FireDamage_h__

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

    class FireDamage : public ComponentBase
    {
    public:
        FireDamage();
        virtual ~FireDamage();
        bool operator ==(const FireDamage& rhs) const;
        bool operator !=(const FireDamage& rhs) const;
        FireDamage(const FireDamage& rhs);
        const FireDamage& operator =(const FireDamage& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        float value;
        float burning_timer;
        bool burning;
        int shooter_id;
        float damage_timer;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_FireDamage_h__
