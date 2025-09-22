#ifndef __mg_ComponentSpirit_h__
#define __mg_ComponentSpirit_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentSpirit : public ComponentBase
    {
    public:
        ComponentSpirit();
        virtual ~ComponentSpirit();
        bool operator ==(const ComponentSpirit& rhs) const;
        bool operator !=(const ComponentSpirit& rhs) const;
        ComponentSpirit(const ComponentSpirit& rhs);
        const ComponentSpirit& operator =(const ComponentSpirit& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        bool initialized;
        float speed_random_t;
        Vector base_point;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentSpirit_h__
