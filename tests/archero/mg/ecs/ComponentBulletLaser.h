#ifndef __mg_ComponentBulletLaser_h__
#define __mg_ComponentBulletLaser_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "ComponentBullet.h"
#include "Segment.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentBulletLaser : public ComponentBullet
    {
    public:
        ComponentBulletLaser();
        virtual ~ComponentBulletLaser();
        bool operator ==(const ComponentBulletLaser& rhs) const;
        bool operator !=(const ComponentBulletLaser& rhs) const;
        ComponentBulletLaser(const ComponentBulletLaser& rhs);
        const ComponentBulletLaser& operator =(const ComponentBulletLaser& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id) override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        Segment segment;
        float duration;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentBulletLaser_h__
