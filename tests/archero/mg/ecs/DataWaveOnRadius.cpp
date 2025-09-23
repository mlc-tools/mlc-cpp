#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataUnit.h"
#include "DataWaveBase.h"
#include "DataWaveOnRadius.h"
#include "ModelEcsBase.h"
#include "SpawnInfo.h"
#include "Transform.h"
#include "UnitStat.h"
#include "Vector.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataWaveOnRadius::TYPE("DataWaveOnRadius");

    DataWaveOnRadius::DataWaveOnRadius()
    : units()
    , min_distance_to_player(0.0f)
    , max_distance_to_player(0.0f)
    {

    }

    DataWaveOnRadius::~DataWaveOnRadius()
    {
    }

    std::vector<SpawnInfo> DataWaveOnRadius::get_units(ModelEcsBase* model) const
    {
        std::vector<SpawnInfo> result;
        assert(max_distance_to_player >= min_distance_to_player);

        auto transform = model->get<Transform>(model->player_id);
        auto count = units.size();
        while(count > 0)
        {
            float radius = random_float() * (max_distance_to_player - min_distance_to_player) + min_distance_to_player;
            assert(radius >= min_distance_to_player && radius <= max_distance_to_player);
            auto pos = transform->position + Vector::generate_random_point_on_radius(radius);
            auto unit_radius = units.at(count-1)->stats.at(UnitStat::size);
            if(!model->ground->has_collision_with_walls(pos, unit_radius))
            {
                count -= 1;
                result.emplace_back(pos, units.at(count));
            }
        }

        return result;
    }

    bool DataWaveOnRadius::operator ==(const DataWaveOnRadius& rhs) const
    {
        bool result = this->DataWaveBase::operator ==(rhs);
        result = result && this->units == rhs.units;
        result = result && this->min_distance_to_player == rhs.min_distance_to_player;
        result = result && this->max_distance_to_player == rhs.max_distance_to_player;
        return result;
    }

    bool DataWaveOnRadius::operator !=(const DataWaveOnRadius& rhs) const
    {
        return !(*this == rhs);
    }

    DataWaveOnRadius::DataWaveOnRadius(const DataWaveOnRadius& rhs)
    {
        this->operator=(rhs);
    }

    const DataWaveOnRadius& DataWaveOnRadius::operator =(const DataWaveOnRadius& rhs)
    {
        this->DataWaveBase::operator=(rhs);
        this->units = rhs.units;
        this->min_distance_to_player = rhs.min_distance_to_player;
        this->max_distance_to_player = rhs.max_distance_to_player;
        return *this;
    }

    std::string DataWaveOnRadius::get_type() const
    {
        return DataWaveOnRadius::TYPE;
    }

    void DataWaveOnRadius::serialize_xml(SerializerXml& serializer) const
    {
        DataWaveBase::serialize_xml(serializer);
        serializer.serialize(units, "units");
        serializer.serialize(min_distance_to_player, "min_distance_to_player", float(0.0f));
        serializer.serialize(max_distance_to_player, "max_distance_to_player", float(0.0f));
    }

    void DataWaveOnRadius::deserialize_xml(DeserializerXml& deserializer)
    {
        DataWaveBase::deserialize_xml(deserializer);
        deserializer.deserialize(units, "units");
        deserializer.deserialize(min_distance_to_player, "min_distance_to_player", float(0.0f));
        deserializer.deserialize(max_distance_to_player, "max_distance_to_player", float(0.0f));
    }

    void DataWaveOnRadius::serialize_json(SerializerJson& serializer) const
    {
        DataWaveBase::serialize_json(serializer);
        serializer.serialize(units, "units");
        serializer.serialize(min_distance_to_player, "min_distance_to_player", float(0.0f));
        serializer.serialize(max_distance_to_player, "max_distance_to_player", float(0.0f));
    }

    void DataWaveOnRadius::deserialize_json(DeserializerJson& deserializer)
    {
        DataWaveBase::deserialize_json(deserializer);
        deserializer.deserialize(units, "units");
        deserializer.deserialize(min_distance_to_player, "min_distance_to_player", float(0.0f));
        deserializer.deserialize(max_distance_to_player, "max_distance_to_player", float(0.0f));
    }

} //namespace mg
