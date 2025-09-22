#ifndef __mg_DataWaveOnRadiusWithInterval_h__
#define __mg_DataWaveOnRadiusWithInterval_h__

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

    class DataWaveOnRadiusWithInterval : public DataWaveBase
    {
    public:
        DataWaveOnRadiusWithInterval();
        virtual ~DataWaveOnRadiusWithInterval();
        virtual bool is_interval_generate() const override;
        virtual bool update(ModelEcsBase* model, float dt) const override;
        virtual void on_start(ModelEcsBase* model) const override;
        virtual bool is_finished(ModelEcsBase* model) const override;
        virtual std::vector<SpawnInfo> get_units(ModelEcsBase* model) const override;
        bool operator ==(const DataWaveOnRadiusWithInterval& rhs) const;
        bool operator !=(const DataWaveOnRadiusWithInterval& rhs) const;
        DataWaveOnRadiusWithInterval(const DataWaveOnRadiusWithInterval& rhs);
        const DataWaveOnRadiusWithInterval& operator =(const DataWaveOnRadiusWithInterval& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::vector<const DataUnit*> units;
        float min_distance_to_player;
        float max_distance_to_player;
        float duration;
        float interval;
        float count_at_once;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataWaveOnRadiusWithInterval_h__
