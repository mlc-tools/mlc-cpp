#ifndef __mg_ComponentStun_h__
#define __mg_ComponentStun_h__

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

    class ComponentStun : public ComponentBase
    {
    public:
        ComponentStun();
        virtual ~ComponentStun();
        bool operator ==(const ComponentStun& rhs) const;
        bool operator !=(const ComponentStun& rhs) const;
        ComponentStun(const ComponentStun& rhs);
        const ComponentStun& operator =(const ComponentStun& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        bool activated;
        float timer;
        float resist_timer;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentStun_h__
