#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ModelBase.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{

    ModelBase::ModelBase()
    {

    }

    ModelBase::~ModelBase(){}

    bool ModelBase::operator ==(const ModelBase& rhs) const
    {
        bool result = true;
        return result;
    }

    bool ModelBase::operator !=(const ModelBase& rhs) const
    {

        return !(*this == rhs);
    }

    ModelBase::ModelBase(const ModelBase& rhs)
    {

        this->operator=(rhs);
    }

    const ModelBase& ModelBase::operator =(const ModelBase& rhs)
    {

        return *this;
    }

    void ModelBase::serialize_xml(SerializerXml& serializer) const
    {

    }

    void ModelBase::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void ModelBase::serialize_json(SerializerJson& serializer) const
    {

    }

    void ModelBase::deserialize_json(DeserializerJson& deserializer)
    {

    }

} // namespace mg
