#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentExp.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentExp::TYPE("ComponentExp");

    ComponentExp::ComponentExp()
    : count(0)
    , clean(false)
    {

    }

    ComponentExp::~ComponentExp()
    {
    }

    bool ComponentExp::operator ==(const ComponentExp& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->count == rhs.count;
        result = result && this->clean == rhs.clean;
        return result;
    }

    bool ComponentExp::operator !=(const ComponentExp& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentExp::ComponentExp(const ComponentExp& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentExp& ComponentExp::operator =(const ComponentExp& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->count = rhs.count;
        this->clean = rhs.clean;
        return *this;
    }

    std::string ComponentExp::get_type() const
    {
        return ComponentExp::TYPE;
    }

    void ComponentExp::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentExp>(this);
    }

    void ComponentExp::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentExp>(this);
    }

    intrusive_ptr<ComponentBase> ComponentExp::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentExp>(id);
    }

    void ComponentExp::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(count, "count", int(0));
        serializer.serialize(clean, "clean", bool(false));
    }

    void ComponentExp::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(count, "count", int(0));
        deserializer.deserialize(clean, "clean", bool(false));
    }

    void ComponentExp::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(count, "count", int(0));
        serializer.serialize(clean, "clean", bool(false));
    }

    void ComponentExp::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(count, "count", int(0));
        deserializer.deserialize(clean, "clean", bool(false));
    }

} //namespace mg
