#ifndef __mg_ComponentCreateUnitsOnDeath_h__
#define __mg_ComponentCreateUnitsOnDeath_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "DataUnit.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class DataUnit;
    class ModelEcsBase;

    class ComponentCreateUnitsOnDeath : public ComponentBase
    {
    public:
        ComponentCreateUnitsOnDeath();
        virtual ~ComponentCreateUnitsOnDeath();
        bool operator ==(const ComponentCreateUnitsOnDeath& rhs) const;
        bool operator !=(const ComponentCreateUnitsOnDeath& rhs) const;
        ComponentCreateUnitsOnDeath(const ComponentCreateUnitsOnDeath& rhs);
        const ComponentCreateUnitsOnDeath& operator =(const ComponentCreateUnitsOnDeath& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::vector<const DataUnit*> units;
        float radius;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentCreateUnitsOnDeath_h__
