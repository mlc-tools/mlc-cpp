#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Modifier.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Modifier::TYPE("Modifier");

    Modifier::Modifier()
    : type(ModifierType::add)
    , value(0.0)
    , string_value("")
    , priority(0)
    , name("")
    , _reference_counter(1)
    {

    }

    Modifier::~Modifier()
    {
    }

    float Modifier::modify(float prev_value) const
    {
        switch(type)
        {
            case ModifierType::add: return prev_value + this->value;
            case ModifierType::sub: return prev_value - this->value;
            case ModifierType::mul: return prev_value * this->value;
            case ModifierType::div: return prev_value / this->value;
        }
        return prev_value;
    }

    void Modifier::retain()
    {
        ++this->_reference_counter;
    }

    int Modifier::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool Modifier::operator ==(const Modifier& rhs) const
    {
        bool result = true;
        result = result && this->type == rhs.type;
        result = result && this->value == rhs.value;
        result = result && this->string_value == rhs.string_value;
        result = result && this->priority == rhs.priority;
        result = result && this->name == rhs.name;
        return result;
    }

    bool Modifier::operator !=(const Modifier& rhs) const
    {
        return !(*this == rhs);
    }

    Modifier::Modifier(const Modifier& rhs)
    {
        this->operator=(rhs);
    }

    const Modifier& Modifier::operator =(const Modifier& rhs)
    {
        this->type = rhs.type;
        this->value = rhs.value;
        this->string_value = rhs.string_value;
        this->priority = rhs.priority;
        this->name = rhs.name;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Modifier::get_type() const
    {
        return Modifier::TYPE;
    }

    void Modifier::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(type, "type");
        serializer.serialize(value, "value", float(0.0));
        serializer.serialize(string_value, "string_value", std::string(""));
        serializer.serialize(priority, "priority", int(0));
        serializer.serialize(name, "name", std::string(""));
    }

    void Modifier::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(type, "type");
        deserializer.deserialize(value, "value", float(0.0));
        deserializer.deserialize(string_value, "string_value", std::string(""));
        deserializer.deserialize(priority, "priority", int(0));
        deserializer.deserialize(name, "name", std::string(""));
    }

    void Modifier::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(type, "type");
        serializer.serialize(value, "value", float(0.0));
        serializer.serialize(string_value, "string_value", std::string(""));
        serializer.serialize(priority, "priority", int(0));
        serializer.serialize(name, "name", std::string(""));
    }

    void Modifier::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(type, "type");
        deserializer.deserialize(value, "value", float(0.0));
        deserializer.deserialize(string_value, "string_value", std::string(""));
        deserializer.deserialize(priority, "priority", int(0));
        deserializer.deserialize(name, "name", std::string(""));
    }

} //namespace mg
