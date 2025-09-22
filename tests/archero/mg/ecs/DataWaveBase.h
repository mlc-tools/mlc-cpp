#ifndef __mg_DataWaveBase_h__
#define __mg_DataWaveBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;
    class SpawnInfo;

    class DataWaveBase
    {
    public:
        DataWaveBase();
        virtual ~DataWaveBase();
        virtual std::vector<SpawnInfo> get_units(ModelEcsBase* model) const = 0;
        virtual bool is_interval_generate() const;
        virtual bool update(ModelEcsBase* model, float dt) const;
        virtual void on_start(ModelEcsBase* model) const;
        virtual bool is_finished(ModelEcsBase* model) const;
        void retain();
        int release();
        bool operator ==(const DataWaveBase& rhs) const;
        bool operator !=(const DataWaveBase& rhs) const;
        DataWaveBase(const DataWaveBase& rhs);
        const DataWaveBase& operator =(const DataWaveBase& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataWaveBase_h__
