#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "DataUnit.h"
#include "SpawnInfo.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SpawnInfo::TYPE("SpawnInfo");

    SpawnInfo::SpawnInfo(const Vector& position, const DataUnit* unit)
    : position()
    , unit(nullptr)
    , _reference_counter(1)
    {
        this->position = position;
        this->unit = unit;
    }

    SpawnInfo::~SpawnInfo()
    {
    }

    void SpawnInfo::retain()
    {
        ++this->_reference_counter;
    }

    int SpawnInfo::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SpawnInfo::operator ==(const SpawnInfo& rhs) const
    {
        bool result = true;
        result = result && this->position == rhs.position;
        result = result && ((this->unit == rhs.unit) || (this->unit != nullptr && rhs.unit != nullptr && *this->unit == *rhs.unit));
        return result;
    }

    bool SpawnInfo::operator !=(const SpawnInfo& rhs) const
    {
        return !(*this == rhs);
    }

    SpawnInfo::SpawnInfo(const SpawnInfo& rhs)
    {
        this->operator=(rhs);
    }

    const SpawnInfo& SpawnInfo::operator =(const SpawnInfo& rhs)
    {
        this->position = rhs.position;
        this->unit = rhs.unit;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SpawnInfo::get_type() const
    {
        return SpawnInfo::TYPE;
    }

    void SpawnInfo::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(position, "position");
        serializer.serialize(unit, "unit");
    }

    void SpawnInfo::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(position, "position");
        deserializer.deserialize(unit, "unit");
    }

    void SpawnInfo::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(position, "position");
        serializer.serialize(unit, "unit");
    }

    void SpawnInfo::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(position, "position");
        deserializer.deserialize(unit, "unit");
    }

} //namespace mg
