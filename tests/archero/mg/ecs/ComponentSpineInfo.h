#ifndef __mg_ComponentSpineInfo_h__
#define __mg_ComponentSpineInfo_h__

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

    class ComponentSpineInfo : public ComponentBase
    {
    public:
        ComponentSpineInfo();
        virtual ~ComponentSpineInfo();
        virtual float get_animation_duration(const std::string& animation) = 0;
        virtual Vector get_bone_position(const std::string& bone) = 0;
        virtual float get_event_time(const std::string& animation, const std::string& event) = 0;
        virtual float get_direction() = 0;
        bool operator ==(const ComponentSpineInfo& rhs) const;
        bool operator !=(const ComponentSpineInfo& rhs) const;
        ComponentSpineInfo(const ComponentSpineInfo& rhs);
        const ComponentSpineInfo& operator =(const ComponentSpineInfo& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentSpineInfo_h__
