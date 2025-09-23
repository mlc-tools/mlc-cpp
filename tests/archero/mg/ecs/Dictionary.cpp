#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Dictionary.h"
#include "Vector.h"
#include <map>
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Dictionary::TYPE("Dictionary");

    Dictionary::Dictionary()
    : strings()
    , vectors()
    , _reference_counter(1)
    {

    }

    Dictionary::~Dictionary()
    {
    }

    void Dictionary::retain()
    {
        ++this->_reference_counter;
    }

    int Dictionary::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool Dictionary::operator ==(const Dictionary& rhs) const
    {
        bool result = true;
        result = result && this->strings == rhs.strings;
        result = result && this->vectors == rhs.vectors;
        return result;
    }

    bool Dictionary::operator !=(const Dictionary& rhs) const
    {
        return !(*this == rhs);
    }

    Dictionary::Dictionary(const Dictionary& rhs)
    {
        this->operator=(rhs);
    }

    const Dictionary& Dictionary::operator =(const Dictionary& rhs)
    {
        this->strings = rhs.strings;
        this->vectors = rhs.vectors;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Dictionary::get_type() const
    {
        return Dictionary::TYPE;
    }

    void Dictionary::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(strings, "strings");
        serializer.serialize(vectors, "vectors");
    }

    void Dictionary::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(strings, "strings");
        deserializer.deserialize(vectors, "vectors");
    }

    void Dictionary::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(strings, "strings");
        serializer.serialize(vectors, "vectors");
    }

    void Dictionary::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(strings, "strings");
        deserializer.deserialize(vectors, "vectors");
    }

} //namespace mg
