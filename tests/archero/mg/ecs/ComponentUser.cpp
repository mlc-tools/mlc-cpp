#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/ModelUser.h"
#include "ComponentBase.h"
#include "ComponentUser.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentUser::TYPE("ComponentUser");

    ComponentUser::ComponentUser()
    : user(nullptr)
    {

    }

    ComponentUser::~ComponentUser()
    {
    }

    bool ComponentUser::operator ==(const ComponentUser& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && ((this->user == rhs.user) || (this->user != nullptr && rhs.user != nullptr && *this->user == *rhs.user));
        return result;
    }

    bool ComponentUser::operator !=(const ComponentUser& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentUser::ComponentUser(const ComponentUser& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentUser& ComponentUser::operator =(const ComponentUser& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->user = rhs.user;
        return *this;
    }

    std::string ComponentUser::get_type() const
    {
        return ComponentUser::TYPE;
    }

    void ComponentUser::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentUser>(this);
    }

    void ComponentUser::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentUser>(this);
    }

    intrusive_ptr<ComponentBase> ComponentUser::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentUser>(id);
    }

    void ComponentUser::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(user, "user");
    }

    void ComponentUser::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(user, "user");
    }

    void ComponentUser::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(user, "user");
    }

    void ComponentUser::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(user, "user");
    }

} //namespace mg
