#ifndef __mg_ComponentGate_h__
#define __mg_ComponentGate_h__

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

    class ComponentGate : public ComponentBase
    {
    public:
        ComponentGate();
        virtual ~ComponentGate();
        bool operator ==(const ComponentGate& rhs) const;
        bool operator !=(const ComponentGate& rhs) const;
        ComponentGate(const ComponentGate& rhs);
        const ComponentGate& operator =(const ComponentGate& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        float radius;
        bool clean;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentGate_h__
