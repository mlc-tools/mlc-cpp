#ifndef __mg_ModelUser_h__
#define __mg_ModelUser_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ModelUserBase.h"
#include "UserProfile.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class UserProfile;

    class ModelUser : public ModelUserBase
    {
    public:
        ModelUser();
        virtual ~ModelUser();
        void update();
        static intrusive_ptr<ModelUser> get_instance();
        virtual void initialize(int current_time);
        void upgrade(int current_time);
        bool is_payer() const;
        void update_profile(UserProfile* profile) const;
        bool operator ==(const ModelUser& rhs) const;
        bool operator !=(const ModelUser& rhs) const;
        ModelUser(const ModelUser& rhs);
        const ModelUser& operator =(const ModelUser& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static intrusive_ptr<ModelUser> instance;

        int session_time_start;
        bool accepted_policy;
        std::vector<std::string> complete_tutorial;
        intrusive_ptr<UserProfile> profile;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ModelUser_h__
