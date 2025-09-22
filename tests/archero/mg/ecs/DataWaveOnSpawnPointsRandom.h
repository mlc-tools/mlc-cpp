#ifndef __mg_DataWaveOnSpawnPointsRandom_h__
#define __mg_DataWaveOnSpawnPointsRandom_h__

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
    class Vector;

    class DataWaveOnSpawnPointsRandom : public DataWaveBase
    {
    public:
        DataWaveOnSpawnPointsRandom();
        virtual ~DataWaveOnSpawnPointsRandom();
        virtual std::vector<SpawnInfo> get_units(ModelEcsBase* model) const override;
        std::vector<Vector> get_points(ModelEcsBase* model) const;
        bool operator ==(const DataWaveOnSpawnPointsRandom& rhs) const;
        bool operator !=(const DataWaveOnSpawnPointsRandom& rhs) const;
        DataWaveOnSpawnPointsRandom(const DataWaveOnSpawnPointsRandom& rhs);
        const DataWaveOnSpawnPointsRandom& operator =(const DataWaveOnSpawnPointsRandom& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::vector<const DataUnit*> units;
        float min_distance_to_player;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataWaveOnSpawnPointsRandom_h__
