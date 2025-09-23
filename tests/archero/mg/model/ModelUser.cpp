#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../data/DataParams.h"
#include "ModelUser.h"
#include "ModelUserBase.h"
#include "System.h"
#include "UserProfile.h"
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    intrusive_ptr<ModelUser> ModelUser::instance(nullptr);
    const std::string ModelUser::TYPE("ModelUser");

    ModelUser::ModelUser()
    : session_time_start(0)
    , accepted_policy(false)
    , complete_tutorial()
    , profile(nullptr)
    {

    }

    ModelUser::~ModelUser()
    {
    }

    void ModelUser::update()
    {

    }

    intrusive_ptr<ModelUser> ModelUser::get_instance()
    {
        assert(nullptr);
        return nullptr;
    }

    void ModelUser::initialize(int current_time)
    {
        ModelUserBase::initialize(current_time);
        this->upgrade(current_time);
    }

    void ModelUser::upgrade(int current_time)
    {
        for(auto __iter__ = this->systems.begin(); __iter__ != this->systems.end();)
        {
            auto& type = __iter__->first; auto& system = __iter__->second; (void)type;(void)system;
            if(system == nullptr) __iter__ = this->systems.erase(__iter__);
            else ++__iter__;
        };
        this->session_time_start = current_time;
        if(!this->profile)
        {
            this->profile = make_intrusive<UserProfile>();
        }

        auto params = DataStorage::shared().get<DataParams>(DataParams::DEFAULT);
        for(auto& system : params->user_systems)
        {
            auto type = system->get_type();
            if(!in_map(type, this->systems))
            {
                this->systems[type] = Factory::shared().build<System>(type);
            }
        }

        for (auto&& pair : this->systems)
        {
            auto& type = pair.first;
            auto& system = pair.second;
            (void)type; //don't generate 'Unused variable' warning
            (void)system; //don't generate 'Unused variable' warning
            system->initialize(this);
        }
    }

    bool ModelUser::is_payer() const
    {
        return false;
    }

    void ModelUser::update_profile(UserProfile* profile) const
    {

    }

    bool ModelUser::operator ==(const ModelUser& rhs) const
    {
        bool result = this->ModelUserBase::operator ==(rhs);
        result = result && this->session_time_start == rhs.session_time_start;
        result = result && this->accepted_policy == rhs.accepted_policy;
        result = result && this->complete_tutorial == rhs.complete_tutorial;
        result = result && ((this->profile == rhs.profile) || (this->profile != nullptr && rhs.profile != nullptr && *this->profile == *rhs.profile));
        return result;
    }

    bool ModelUser::operator !=(const ModelUser& rhs) const
    {
        return !(*this == rhs);
    }

    ModelUser::ModelUser(const ModelUser& rhs)
    {
        this->operator=(rhs);
    }

    const ModelUser& ModelUser::operator =(const ModelUser& rhs)
    {
        this->ModelUserBase::operator=(rhs);
        this->session_time_start = rhs.session_time_start;
        this->accepted_policy = rhs.accepted_policy;
        this->complete_tutorial = rhs.complete_tutorial;
        this->profile = rhs.profile;
        return *this;
    }

    std::string ModelUser::get_type() const
    {
        return ModelUser::TYPE;
    }

    void ModelUser::serialize_xml(SerializerXml& serializer) const
    {
        ModelUserBase::serialize_xml(serializer);
        serializer.serialize(accepted_policy, "accepted_policy", bool(false));
        serializer.serialize(complete_tutorial, "complete_tutorial");
        serializer.serialize(profile, "profile");
    }

    void ModelUser::deserialize_xml(DeserializerXml& deserializer)
    {
        ModelUserBase::deserialize_xml(deserializer);
        deserializer.deserialize(accepted_policy, "accepted_policy", bool(false));
        deserializer.deserialize(complete_tutorial, "complete_tutorial");
        deserializer.deserialize(profile, "profile");
    }

    void ModelUser::serialize_json(SerializerJson& serializer) const
    {
        ModelUserBase::serialize_json(serializer);
        serializer.serialize(accepted_policy, "accepted_policy", bool(false));
        serializer.serialize(complete_tutorial, "complete_tutorial");
        serializer.serialize(profile, "profile");
    }

    void ModelUser::deserialize_json(DeserializerJson& deserializer)
    {
        ModelUserBase::deserialize_json(deserializer);
        deserializer.deserialize(accepted_policy, "accepted_policy", bool(false));
        deserializer.deserialize(complete_tutorial, "complete_tutorial");
        deserializer.deserialize(profile, "profile");
    }

} //namespace mg
