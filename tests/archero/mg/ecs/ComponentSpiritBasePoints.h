#ifndef __mg_ComponentSpiritBasePoints_h__
#define __mg_ComponentSpiritBasePoints_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "Vector.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;
    class Vector;

    class ComponentSpiritBasePoints : public ComponentBase
    {
    public:
        ComponentSpiritBasePoints();
        virtual ~ComponentSpiritBasePoints();
        bool operator ==(const ComponentSpiritBasePoints& rhs) const;
        bool operator !=(const ComponentSpiritBasePoints& rhs) const;
        ComponentSpiritBasePoints(const ComponentSpiritBasePoints& rhs);
        const ComponentSpiritBasePoints& operator =(const ComponentSpiritBasePoints& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::map<int, Vector> points;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentSpiritBasePoints_h__
