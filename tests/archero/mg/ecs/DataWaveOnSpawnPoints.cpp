#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataUnit.h"
#include "DataWaveOnSpawnPoints.h"
#include "ModelEcsBase.h"
#include "SpawnInfo.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataWaveOnSpawnPoints::TYPE("DataWaveOnSpawnPoints");

    DataWaveOnSpawnPoints::DataWaveOnSpawnPoints()
    : units()
    {

    }

    DataWaveOnSpawnPoints::~DataWaveOnSpawnPoints()
    {
    }

    std::vector<SpawnInfo> DataWaveOnSpawnPoints::get_units(ModelEcsBase* model) const
    {
        std::vector<SpawnInfo> result;
        for (auto&& pair : this->units)
        {
            auto& name = pair.first;
            auto& unit = pair.second;
            (void)name; //don't generate 'Unused variable' warning
            (void)unit; //don't generate 'Unused variable' warning
            result.push_back(SpawnInfo(model->spawn_points.at(name), unit));
        }
        return result;
    }

    bool DataWaveOnSpawnPoints::operator ==(const DataWaveOnSpawnPoints& rhs) const
    {
        bool result = this->DataWaveBase::operator ==(rhs);
        result = result && this->units == rhs.units;
        return result;
    }

    bool DataWaveOnSpawnPoints::operator !=(const DataWaveOnSpawnPoints& rhs) const
    {
        return !(*this == rhs);
    }

    DataWaveOnSpawnPoints::DataWaveOnSpawnPoints(const DataWaveOnSpawnPoints& rhs)
    {
        this->operator=(rhs);
    }

    const DataWaveOnSpawnPoints& DataWaveOnSpawnPoints::operator =(const DataWaveOnSpawnPoints& rhs)
    {
        this->DataWaveBase::operator=(rhs);
        this->units = rhs.units;
        return *this;
    }

    std::string DataWaveOnSpawnPoints::get_type() const
    {
        return DataWaveOnSpawnPoints::TYPE;
    }

    void DataWaveOnSpawnPoints::serialize_xml(SerializerXml& serializer) const
    {
        DataWaveBase::serialize_xml(serializer);
        serializer.serialize(units, "units");
    }

    void DataWaveOnSpawnPoints::deserialize_xml(DeserializerXml& deserializer)
    {
        DataWaveBase::deserialize_xml(deserializer);
        deserializer.deserialize(units, "units");
    }

    void DataWaveOnSpawnPoints::serialize_json(SerializerJson& serializer) const
    {
        DataWaveBase::serialize_json(serializer);
        serializer.serialize(units, "units");
    }

    void DataWaveOnSpawnPoints::deserialize_json(DeserializerJson& deserializer)
    {
        DataWaveBase::deserialize_json(deserializer);
        deserializer.deserialize(units, "units");
    }

} //namespace mg
