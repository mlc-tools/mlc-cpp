#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataWaveBase.h"
#include "ModelEcsBase.h"
#include "SpawnInfo.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataWaveBase::TYPE("DataWaveBase");

    DataWaveBase::DataWaveBase()
    : _reference_counter(1)
    {

    }

    DataWaveBase::~DataWaveBase()
    {
    }

    bool DataWaveBase::is_interval_generate() const
    {
        return false;
    }

    bool DataWaveBase::update(ModelEcsBase* model, float dt) const
    {
        return false;
    }

    void DataWaveBase::on_start(ModelEcsBase* model) const
    {

    }

    bool DataWaveBase::is_finished(ModelEcsBase* model) const
    {
        return false;
    }

    void DataWaveBase::retain()
    {
        ++this->_reference_counter;
    }

    int DataWaveBase::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool DataWaveBase::operator ==(const DataWaveBase& rhs) const
    {
        bool result = true;
        return result;
    }

    bool DataWaveBase::operator !=(const DataWaveBase& rhs) const
    {
        return !(*this == rhs);
    }

    DataWaveBase::DataWaveBase(const DataWaveBase& rhs)
    {
        this->operator=(rhs);
    }

    const DataWaveBase& DataWaveBase::operator =(const DataWaveBase& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataWaveBase::get_type() const
    {
        return DataWaveBase::TYPE;
    }

    void DataWaveBase::serialize_xml(SerializerXml& serializer) const
    {

    }

    void DataWaveBase::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void DataWaveBase::serialize_json(SerializerJson& serializer) const
    {

    }

    void DataWaveBase::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
