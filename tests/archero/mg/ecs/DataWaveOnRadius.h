#ifndef __mg_DataWaveOnRadius_h__
#define __mg_DataWaveOnRadius_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "DataUnit.h"
#include "DataWaveBase.h"
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
    class SpawnInfo;

    class DataWaveOnRadius : public DataWaveBase
    {
    public:
        DataWaveOnRadius();
        virtual ~DataWaveOnRadius();
        virtual std::vector<SpawnInfo> get_units(ModelEcsBase* model) const override;
        bool operator ==(const DataWaveOnRadius& rhs) const;
        bool operator !=(const DataWaveOnRadius& rhs) const;
        DataWaveOnRadius(const DataWaveOnRadius& rhs);
        const DataWaveOnRadius& operator =(const DataWaveOnRadius& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::vector<const DataUnit*> units;
        float min_distance_to_player;
        float max_distance_to_player;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataWaveOnRadius_h__
