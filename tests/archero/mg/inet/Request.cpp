#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "CommandBase.h"
#include "Request.h"
#include "ResponseError.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Request::TYPE("Request");

    Request::Request()
    : auth_key("")
    , user_id(0)
    , version("")
    , current_time(0)
    {

    }

    Request::~Request()
    {
    }

    bool Request::is_online() const
    {
        return false;
    }

    std::string Request::build_payload() const
    {
        return "";
    }

    intrusive_ptr<CommandBase> Request::execute()
    {
        return nullptr;
    }

    intrusive_ptr<ResponseError> Request::error(const std::string& message)
    {
        auto response = make_intrusive<ResponseError>();
        response->error = message;
        return response;
    }

    bool Request::operator ==(const Request& rhs) const
    {
        bool result = this->CommandBase::operator ==(rhs);
        result = result && this->auth_key == rhs.auth_key;
        result = result && this->user_id == rhs.user_id;
        result = result && this->version == rhs.version;
        result = result && this->current_time == rhs.current_time;
        return result;
    }

    bool Request::operator !=(const Request& rhs) const
    {
        return !(*this == rhs);
    }

    Request::Request(const Request& rhs)
    {
        this->operator=(rhs);
    }

    const Request& Request::operator =(const Request& rhs)
    {
        this->CommandBase::operator=(rhs);
        this->auth_key = rhs.auth_key;
        this->user_id = rhs.user_id;
        this->version = rhs.version;
        this->current_time = rhs.current_time;
        return *this;
    }

    std::string Request::get_type() const
    {
        return Request::TYPE;
    }

    void Request::serialize_xml(SerializerXml& serializer) const
    {
        CommandBase::serialize_xml(serializer);
        serializer.serialize(auth_key, "auth_key", std::string(""));
        serializer.serialize(user_id, "user_id", int(0));
        serializer.serialize(version, "version", std::string(""));
    }

    void Request::deserialize_xml(DeserializerXml& deserializer)
    {
        CommandBase::deserialize_xml(deserializer);
        deserializer.deserialize(auth_key, "auth_key", std::string(""));
        deserializer.deserialize(user_id, "user_id", int(0));
        deserializer.deserialize(version, "version", std::string(""));
    }

    void Request::serialize_json(SerializerJson& serializer) const
    {
        CommandBase::serialize_json(serializer);
        serializer.serialize(auth_key, "auth_key", std::string(""));
        serializer.serialize(user_id, "user_id", int(0));
        serializer.serialize(version, "version", std::string(""));
    }

    void Request::deserialize_json(DeserializerJson& deserializer)
    {
        CommandBase::deserialize_json(deserializer);
        deserializer.deserialize(auth_key, "auth_key", std::string(""));
        deserializer.deserialize(user_id, "user_id", int(0));
        deserializer.deserialize(version, "version", std::string(""));
    }

} //namespace mg
