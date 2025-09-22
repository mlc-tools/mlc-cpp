#ifndef __mg_UserProfile_h__
#define __mg_UserProfile_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Model.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class UserProfile : public Model
    {
    public:
        UserProfile();
        virtual ~UserProfile();
        bool operator ==(const UserProfile& rhs) const;
        bool operator !=(const UserProfile& rhs) const;
        UserProfile(const UserProfile& rhs);
        const UserProfile& operator =(const UserProfile& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::string name;
        bool is_base_name;
        int id;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_UserProfile_h__
