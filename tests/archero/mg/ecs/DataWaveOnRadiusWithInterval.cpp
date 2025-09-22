#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataUnit.h"
#include "DataWaveOnRadiusWithInterval.h"
#include "ModelEcsBase.h"
#include "SpawnInfo.h"
#include "Transform.h"
#include "UnitStat.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataWaveOnRadiusWithInterval::TYPE("DataWaveOnRadiusWithInterval");

    DataWaveOnRadiusWithInterval::DataWaveOnRadiusWithInterval()
    : units()
    , min_distance_to_player(0.0)
    , max_distance_to_player(0.0)
    , duration(60)
    , interval(1)
    , count_at_once(1)
    {

    }

    DataWaveOnRadiusWithInterval::~DataWaveOnRadiusWithInterval()
    {
    }

    bool DataWaveOnRadiusWithInterval::is_interval_generate() const
    {
        return true;
    }

    bool DataWaveOnRadiusWithInterval::update(ModelEcsBase* model, float dt) const
    {
        model->timer_wave_interval += dt;
        model->timer_wave_duration += dt;
        if(model->timer_wave_interval >= this->interval)
        {
            model->timer_wave_interval = 0;
            return true;
        }
        return false;
    }

    void DataWaveOnRadiusWithInterval::on_start(ModelEcsBase* model) const
    {
        model->timer_wave_interval = 0;
        model->timer_wave_duration = 0;
    }

    bool DataWaveOnRadiusWithInterval::is_finished(ModelEcsBase* model) const
    {
        return model->timer_wave_duration >= this->duration;
    }

    std::vector<SpawnInfo> DataWaveOnRadiusWithInterval::get_units(ModelEcsBase* model) const
    {
        std::vector<SpawnInfo> result;
        assert(max_distance_to_player >= min_distance_to_player);
        auto transform = model->get<Transform>(model->player_id);
        auto count = this->count_at_once;
        while(count > 0)
        {
            float radius = random_float() * (max_distance_to_player - min_distance_to_player) + min_distance_to_player;
            assert(radius >= min_distance_to_player && radius <= max_distance_to_player);
            auto pos = transform->position + Vector::generate_random_point_on_radius(radius);
            auto rand_index = random_int(0, list_size(this->units));
            auto unit_radius = units.at(rand_index)->stats.at(UnitStat::size);
            if(!model->ground->has_collision_with_walls(pos, unit_radius))
            {
                count -= 1;
                result.emplace_back(pos, units.at(rand_index));
            }
        }
        return result;
    }

    bool DataWaveOnRadiusWithInterval::operator ==(const DataWaveOnRadiusWithInterval& rhs) const
    {
        bool result = this->DataWaveBase::operator ==(rhs);
        result = result && this->units == rhs.units;
        result = result && this->min_distance_to_player == rhs.min_distance_to_player;
        result = result && this->max_distance_to_player == rhs.max_distance_to_player;
        result = result && this->duration == rhs.duration;
        result = result && this->interval == rhs.interval;
        result = result && this->count_at_once == rhs.count_at_once;
        return result;
    }

    bool DataWaveOnRadiusWithInterval::operator !=(const DataWaveOnRadiusWithInterval& rhs) const
    {
        return !(*this == rhs);
    }

    DataWaveOnRadiusWithInterval::DataWaveOnRadiusWithInterval(const DataWaveOnRadiusWithInterval& rhs)
    {
        this->operator=(rhs);
    }

    const DataWaveOnRadiusWithInterval& DataWaveOnRadiusWithInterval::operator =(const DataWaveOnRadiusWithInterval& rhs)
    {
        this->DataWaveBase::operator=(rhs);
        this->units = rhs.units;
        this->min_distance_to_player = rhs.min_distance_to_player;
        this->max_distance_to_player = rhs.max_distance_to_player;
        this->duration = rhs.duration;
        this->interval = rhs.interval;
        this->count_at_once = rhs.count_at_once;
        return *this;
    }

    std::string DataWaveOnRadiusWithInterval::get_type() const
    {
        return DataWaveOnRadiusWithInterval::TYPE;
    }

    void DataWaveOnRadiusWithInterval::serialize_xml(SerializerXml& serializer) const
    {
        DataWaveBase::serialize_xml(serializer);
        serializer.serialize(units, "units");
        serializer.serialize(min_distance_to_player, "min_distance_to_player", float(0.0));
        serializer.serialize(max_distance_to_player, "max_distance_to_player", float(0.0));
        serializer.serialize(duration, "duration", float(60));
        serializer.serialize(interval, "interval", float(1));
        serializer.serialize(count_at_once, "count_at_once", float(1));
    }

    void DataWaveOnRadiusWithInterval::deserialize_xml(DeserializerXml& deserializer)
    {
        DataWaveBase::deserialize_xml(deserializer);
        deserializer.deserialize(units, "units");
        deserializer.deserialize(min_distance_to_player, "min_distance_to_player", float(0.0));
        deserializer.deserialize(max_distance_to_player, "max_distance_to_player", float(0.0));
        deserializer.deserialize(duration, "duration", float(60));
        deserializer.deserialize(interval, "interval", float(1));
        deserializer.deserialize(count_at_once, "count_at_once", float(1));
    }

    void DataWaveOnRadiusWithInterval::serialize_json(SerializerJson& serializer) const
    {
        DataWaveBase::serialize_json(serializer);
        serializer.serialize(units, "units");
        serializer.serialize(min_distance_to_player, "min_distance_to_player", float(0.0));
        serializer.serialize(max_distance_to_player, "max_distance_to_player", float(0.0));
        serializer.serialize(duration, "duration", float(60));
        serializer.serialize(interval, "interval", float(1));
        serializer.serialize(count_at_once, "count_at_once", float(1));
    }

    void DataWaveOnRadiusWithInterval::deserialize_json(DeserializerJson& deserializer)
    {
        DataWaveBase::deserialize_json(deserializer);
        deserializer.deserialize(units, "units");
        deserializer.deserialize(min_distance_to_player, "min_distance_to_player", float(0.0));
        deserializer.deserialize(max_distance_to_player, "max_distance_to_player", float(0.0));
        deserializer.deserialize(duration, "duration", float(60));
        deserializer.deserialize(interval, "interval", float(1));
        deserializer.deserialize(count_at_once, "count_at_once", float(1));
    }

} //namespace mg
