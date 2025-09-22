#ifndef __mg_ComponentSphereSpawn_h__
#define __mg_ComponentSphereSpawn_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "DataUnit.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentSphereSpawn : public ComponentBase
    {
    public:
        ComponentSphereSpawn();
        virtual ~ComponentSphereSpawn();
        bool operator ==(const ComponentSphereSpawn& rhs) const;
        bool operator !=(const ComponentSphereSpawn& rhs) const;
        ComponentSphereSpawn(const ComponentSphereSpawn& rhs);
        const ComponentSphereSpawn& operator =(const ComponentSphereSpawn& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        bool spawned;
        int count;
        const DataUnit* data;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentSphereSpawn_h__
