#ifndef __mg_DataStatUpgrade_h__
#define __mg_DataStatUpgrade_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "DataStatUpgradeVisual.h"
#include "Modifier.h"
#include "SkillRarity.h"
#include "UnitStat.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ComponentBase;
    class Modifier;
    class UnitStat;

    class DataStatUpgrade
    {
    public:
        DataStatUpgrade();
        ~DataStatUpgrade();
        void retain();
        int release();
        bool operator ==(const DataStatUpgrade& rhs) const;
        bool operator !=(const DataStatUpgrade& rhs) const;
        DataStatUpgrade(const DataStatUpgrade& rhs);
        const DataStatUpgrade& operator =(const DataStatUpgrade& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string name;
        SkillRarity rarity;
        std::vector<UnitStat> stats;
        std::vector<Modifier> modifiers;
        std::vector<intrusive_ptr<ComponentBase>> components;
        bool to_hero;
        bool to_spirits;
        DataStatUpgradeVisual visual;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataStatUpgrade_h__
