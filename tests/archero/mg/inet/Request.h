#ifndef __mg_Request_h__
#define __mg_Request_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "CommandBase.h"
#include "ResponseError.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class Request : public CommandBase
    {
    public:
        Request();
        virtual ~Request();
        bool is_online() const;
        virtual std::string build_payload() const;
        intrusive_ptr<CommandBase> execute();
        intrusive_ptr<ResponseError> error(const std::string& message);
        bool operator ==(const Request& rhs) const;
        bool operator !=(const Request& rhs) const;
        Request(const Request& rhs);
        const Request& operator =(const Request& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::string auth_key;
        int user_id;
        std::string version;
        int current_time;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Request_h__
