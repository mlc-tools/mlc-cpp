#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Shuffle.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Shuffle::TYPE("Shuffle");

    Shuffle::Shuffle()
    : _reference_counter(1)
    {

    }

    Shuffle::~Shuffle()
    {
    }

    void Shuffle::retain()
    {
        ++this->_reference_counter;
    }

    int Shuffle::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool Shuffle::operator ==(const Shuffle& rhs) const
    {
        bool result = true;
        return result;
    }

    bool Shuffle::operator !=(const Shuffle& rhs) const
    {
        return !(*this == rhs);
    }

    Shuffle::Shuffle(const Shuffle& rhs)
    {
        this->operator=(rhs);
    }

    const Shuffle& Shuffle::operator =(const Shuffle& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Shuffle::get_type() const
    {
        return Shuffle::TYPE;
    }

    void Shuffle::serialize_xml(SerializerXml& serializer) const
    {

    }

    void Shuffle::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void Shuffle::serialize_json(SerializerJson& serializer) const
    {

    }

    void Shuffle::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
