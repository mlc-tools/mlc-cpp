#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Model.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Model::TYPE("Model");

    Model::Model()
    : initialized(false)
    , timestamp(0)
    , ready_to_write(false)
    , _reference_counter(1)
    {

    }

    Model::~Model()
    {
    }

    void Model::initialize(int current_time)
    {
        this->initialized = true;
        this->timestamp = current_time;
    }

    bool Model::is_outdate(int current_time)
    {
        return current_time - this->timestamp > 300;
    }

    void Model::retain()
    {
        ++this->_reference_counter;
    }

    int Model::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool Model::operator ==(const Model& rhs) const
    {
        bool result = true;
        result = result && this->initialized == rhs.initialized;
        result = result && this->timestamp == rhs.timestamp;
        result = result && this->ready_to_write == rhs.ready_to_write;
        return result;
    }

    bool Model::operator !=(const Model& rhs) const
    {
        return !(*this == rhs);
    }

    Model::Model(const Model& rhs)
    {
        this->operator=(rhs);
    }

    const Model& Model::operator =(const Model& rhs)
    {
        this->initialized = rhs.initialized;
        this->timestamp = rhs.timestamp;
        this->ready_to_write = rhs.ready_to_write;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Model::get_type() const
    {
        return Model::TYPE;
    }

    void Model::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(initialized, "initialized", bool(false));
    }

    void Model::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(initialized, "initialized", bool(false));
    }

    void Model::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(initialized, "initialized", bool(false));
    }

    void Model::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(initialized, "initialized", bool(false));
    }

} //namespace mg
