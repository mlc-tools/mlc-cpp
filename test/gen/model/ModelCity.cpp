#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ModelBase.h"
#include "ModelCity.h"
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    std::map<std::string, std::string> ModelCity::test_map;
    std::vector<std::string> ModelCity::test_list;
    ModelBase ModelCity::model_base_static;
    const std::string ModelCity::TYPE("ModelCity");

    ModelCity::ModelCity()
    : id(0)
    , owner(0)
    {

    }

    ModelCity::~ModelCity(){}

    int ModelCity::some_function(const std::string& resource)
    {

        return 0;

    }

    int ModelCity::some_private_function(const std::string& resource)
    {

        return 0;

    }

    bool ModelCity::operator ==(const ModelCity& rhs) const
    {

        bool result = this->ModelBase::operator ==(rhs);
        result = result && this->id == rhs.id;
        result = result && this->owner == rhs.owner;
        return result;
    }

    bool ModelCity::operator !=(const ModelCity& rhs) const
    {

        return !(*this == rhs);
    }

    ModelCity::ModelCity(const ModelCity& rhs)
    {

        this->operator=(rhs);
    }

    const ModelCity& ModelCity::operator =(const ModelCity& rhs)
    {

        this->ModelBase::operator=(rhs);
        this->id = rhs.id;
        this->owner = rhs.owner;
        return *this;
    }

    std::string ModelCity::get_type() const
    {
        return ModelCity::TYPE;
    }

    void ModelCity::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(id, "id", int(0));
        serializer.serialize(owner, "owner", int(0));
    }

    void ModelCity::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(id, "id", int(0));
        deserializer.deserialize(owner, "owner", int(0));
    }

    void ModelCity::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(id, "id", int(0));
        serializer.serialize(owner, "owner", int(0));
    }

    void ModelCity::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(id, "id", int(0));
        deserializer.deserialize(owner, "owner", int(0));
    }

} // namespace mg
