#ifndef __mg_DataUnit_h__
#define __mg_DataUnit_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "BulletType.h"
#include "ComponentBase.h"
#include "DataUnitVisual.h"
#include "UnitStat.h"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ComponentBase;
    class UnitStat;

    class DataUnit
    {
    public:
        DataUnit();
        ~DataUnit();
        void retain();
        int release();
        bool operator ==(const DataUnit& rhs) const;
        bool operator !=(const DataUnit& rhs) const;
        DataUnit(const DataUnit& rhs);
        const DataUnit& operator =(const DataUnit& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string name;
        BulletType bullet_type;
        DataUnitVisual visual;
        std::map<UnitStat, float> stats;
        std::map<UnitStat, std::string> stats_string;
        std::vector<intrusive_ptr<ComponentBase>> components;
        bool is_boss;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataUnit_h__
