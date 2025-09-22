#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "UserProfile.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string UserProfile::TYPE("UserProfile");

    UserProfile::UserProfile()
    : name("Player")
    , is_base_name(true)
    , id(0)
    {

    }

    UserProfile::~UserProfile()
    {
    }

    bool UserProfile::operator ==(const UserProfile& rhs) const
    {
        bool result = this->Model::operator ==(rhs);
        result = result && this->name == rhs.name;
        result = result && this->is_base_name == rhs.is_base_name;
        result = result && this->id == rhs.id;
        return result;
    }

    bool UserProfile::operator !=(const UserProfile& rhs) const
    {
        return !(*this == rhs);
    }

    UserProfile::UserProfile(const UserProfile& rhs)
    {
        this->operator=(rhs);
    }

    const UserProfile& UserProfile::operator =(const UserProfile& rhs)
    {
        this->Model::operator=(rhs);
        this->name = rhs.name;
        this->is_base_name = rhs.is_base_name;
        this->id = rhs.id;
        return *this;
    }

    std::string UserProfile::get_type() const
    {
        return UserProfile::TYPE;
    }

    void UserProfile::serialize_xml(SerializerXml& serializer) const
    {
        Model::serialize_xml(serializer);
        serializer.serialize(name, "name", std::string("Player"));
        serializer.serialize(is_base_name, "is_base_name", bool(true));
        serializer.serialize(id, "id", int(0));
    }

    void UserProfile::deserialize_xml(DeserializerXml& deserializer)
    {
        Model::deserialize_xml(deserializer);
        deserializer.deserialize(name, "name", std::string("Player"));
        deserializer.deserialize(is_base_name, "is_base_name", bool(true));
        deserializer.deserialize(id, "id", int(0));
    }

    void UserProfile::serialize_json(SerializerJson& serializer) const
    {
        Model::serialize_json(serializer);
        serializer.serialize(name, "name", std::string("Player"));
        serializer.serialize(is_base_name, "is_base_name", bool(true));
        serializer.serialize(id, "id", int(0));
    }

    void UserProfile::deserialize_json(DeserializerJson& deserializer)
    {
        Model::deserialize_json(deserializer);
        deserializer.deserialize(name, "name", std::string("Player"));
        deserializer.deserialize(is_base_name, "is_base_name", bool(true));
        deserializer.deserialize(id, "id", int(0));
    }

} //namespace mg
