#ifndef __mg_DataWaveOnSpawnPoints_h__
#define __mg_DataWaveOnSpawnPoints_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "DataUnit.h"
#include "DataWaveBase.h"
#include <map>
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

    class DataWaveOnSpawnPoints : public DataWaveBase
    {
    public:
        DataWaveOnSpawnPoints();
        virtual ~DataWaveOnSpawnPoints();
        virtual std::vector<SpawnInfo> get_units(ModelEcsBase* model) const override;
        bool operator ==(const DataWaveOnSpawnPoints& rhs) const;
        bool operator !=(const DataWaveOnSpawnPoints& rhs) const;
        DataWaveOnSpawnPoints(const DataWaveOnSpawnPoints& rhs);
        const DataWaveOnSpawnPoints& operator =(const DataWaveOnSpawnPoints& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::map<std::string, const DataUnit*> units;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataWaveOnSpawnPoints_h__
