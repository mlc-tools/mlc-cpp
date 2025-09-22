#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataUnit.h"
#include "DataWaveOnSpawnPointsRandom.h"
#include "ModelEcsBase.h"
#include "SpawnInfo.h"
#include "Transform.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataWaveOnSpawnPointsRandom::TYPE("DataWaveOnSpawnPointsRandom");

    DataWaveOnSpawnPointsRandom::DataWaveOnSpawnPointsRandom()
    : units()
    , min_distance_to_player(0.0)
    {

    }

    DataWaveOnSpawnPointsRandom::~DataWaveOnSpawnPointsRandom()
    {
    }

    std::vector<SpawnInfo> DataWaveOnSpawnPointsRandom::get_units(ModelEcsBase* model) const
    {
        std::vector<SpawnInfo> result;
        auto points = this->get_points(model);
        for(auto unit : this->units)
        {
            if(points.empty())
            points = this->get_points(model);
            assert(!points.empty());
            auto index = random_int(0, points.size());
            result.push_back(SpawnInfo(points.at(index), unit));
            list_erase(points, index);
        }
        return result;
    }

    std::vector<Vector> DataWaveOnSpawnPointsRandom::get_points(ModelEcsBase* model) const
    {
        std::vector<Vector> result;
        float dist_sq = this->min_distance_to_player * this->min_distance_to_player;
        auto transform = model->get<Transform>(model->player_id);
        for (auto&& pair : model->spawn_points)
        {
            auto& _ = pair.first;
            auto& pos = pair.second;
            (void)_; //don't generate 'Unused variable' warning
            (void)pos; //don't generate 'Unused variable' warning
            if(transform->position.get_distance_sq(pos) <= dist_sq)
            {
                result.push_back(pos);
            }
        }
        return result;
    }

    bool DataWaveOnSpawnPointsRandom::operator ==(const DataWaveOnSpawnPointsRandom& rhs) const
    {
        bool result = this->DataWaveBase::operator ==(rhs);
        result = result && this->units == rhs.units;
        result = result && this->min_distance_to_player == rhs.min_distance_to_player;
        return result;
    }

    bool DataWaveOnSpawnPointsRandom::operator !=(const DataWaveOnSpawnPointsRandom& rhs) const
    {
        return !(*this == rhs);
    }

    DataWaveOnSpawnPointsRandom::DataWaveOnSpawnPointsRandom(const DataWaveOnSpawnPointsRandom& rhs)
    {
        this->operator=(rhs);
    }

    const DataWaveOnSpawnPointsRandom& DataWaveOnSpawnPointsRandom::operator =(const DataWaveOnSpawnPointsRandom& rhs)
    {
        this->DataWaveBase::operator=(rhs);
        this->units = rhs.units;
        this->min_distance_to_player = rhs.min_distance_to_player;
        return *this;
    }

    std::string DataWaveOnSpawnPointsRandom::get_type() const
    {
        return DataWaveOnSpawnPointsRandom::TYPE;
    }

    void DataWaveOnSpawnPointsRandom::serialize_xml(SerializerXml& serializer) const
    {
        DataWaveBase::serialize_xml(serializer);
        serializer.serialize(units, "units");
        serializer.serialize(min_distance_to_player, "min_distance_to_player", float(0.0));
    }

    void DataWaveOnSpawnPointsRandom::deserialize_xml(DeserializerXml& deserializer)
    {
        DataWaveBase::deserialize_xml(deserializer);
        deserializer.deserialize(units, "units");
        deserializer.deserialize(min_distance_to_player, "min_distance_to_player", float(0.0));
    }

    void DataWaveOnSpawnPointsRandom::serialize_json(SerializerJson& serializer) const
    {
        DataWaveBase::serialize_json(serializer);
        serializer.serialize(units, "units");
        serializer.serialize(min_distance_to_player, "min_distance_to_player", float(0.0));
    }

    void DataWaveOnSpawnPointsRandom::deserialize_json(DeserializerJson& deserializer)
    {
        DataWaveBase::deserialize_json(deserializer);
        deserializer.deserialize(units, "units");
        deserializer.deserialize(min_distance_to_player, "min_distance_to_player", float(0.0));
    }

} //namespace mg
