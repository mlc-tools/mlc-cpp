#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ModelUserBase.h"
#include "System.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ModelUserBase::TYPE("ModelUserBase");

    ModelUserBase::ModelUserBase()
    : id(0)
    , auth_key("")
    , auth_key_for_tests("")
    , systems()
    {

    }

    ModelUserBase::~ModelUserBase()
    {
    }

    void ModelUserBase::copy_system(System* system)
    {
        this->systems[system->get_type()] = system;
    }

    std::string ModelUserBase::get_auth_key() const
    {
        return this->auth_key;
    }

    bool ModelUserBase::operator ==(const ModelUserBase& rhs) const
    {
        bool result = this->Model::operator ==(rhs);
        result = result && this->id == rhs.id;
        result = result && this->auth_key == rhs.auth_key;
        result = result && this->auth_key_for_tests == rhs.auth_key_for_tests;
        result = result && this->systems == rhs.systems;
        return result;
    }

    bool ModelUserBase::operator !=(const ModelUserBase& rhs) const
    {
        return !(*this == rhs);
    }

    ModelUserBase::ModelUserBase(const ModelUserBase& rhs)
    {
        this->operator=(rhs);
    }

    const ModelUserBase& ModelUserBase::operator =(const ModelUserBase& rhs)
    {
        this->Model::operator=(rhs);
        this->id = rhs.id;
        this->auth_key = rhs.auth_key;
        this->auth_key_for_tests = rhs.auth_key_for_tests;
        this->systems = rhs.systems;
        return *this;
    }

    std::string ModelUserBase::get_type() const
    {
        return ModelUserBase::TYPE;
    }

    void ModelUserBase::serialize_xml(SerializerXml& serializer) const
    {
        Model::serialize_xml(serializer);
        serializer.serialize(id, "id", int(0));
        serializer.serialize(auth_key, "auth_key", std::string(""));
        serializer.serialize(auth_key_for_tests, "auth_key_for_tests", std::string(""));
        serializer.serialize(systems, "systems");
    }

    void ModelUserBase::deserialize_xml(DeserializerXml& deserializer)
    {
        Model::deserialize_xml(deserializer);
        deserializer.deserialize(id, "id", int(0));
        deserializer.deserialize(auth_key, "auth_key", std::string(""));
        deserializer.deserialize(auth_key_for_tests, "auth_key_for_tests", std::string(""));
        deserializer.deserialize(systems, "systems");
    }

    void ModelUserBase::serialize_json(SerializerJson& serializer) const
    {
        Model::serialize_json(serializer);
        serializer.serialize(id, "id", int(0));
        serializer.serialize(auth_key, "auth_key", std::string(""));
        serializer.serialize(auth_key_for_tests, "auth_key_for_tests", std::string(""));
        serializer.serialize(systems, "systems");
    }

    void ModelUserBase::deserialize_json(DeserializerJson& deserializer)
    {
        Model::deserialize_json(deserializer);
        deserializer.deserialize(id, "id", int(0));
        deserializer.deserialize(auth_key, "auth_key", std::string(""));
        deserializer.deserialize(auth_key_for_tests, "auth_key_for_tests", std::string(""));
        deserializer.deserialize(systems, "systems");
    }

} //namespace mg
