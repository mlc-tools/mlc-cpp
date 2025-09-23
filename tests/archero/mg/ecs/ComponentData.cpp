#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentData.h"
#include "DataUnit.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentData::TYPE("ComponentData");

    ComponentData::ComponentData(const DataUnit* data)
    : data(nullptr)
    {
        this->data = data;
    }

    ComponentData::~ComponentData()
    {
    }

    bool ComponentData::operator ==(const ComponentData& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && ((this->data == rhs.data) || (this->data != nullptr && rhs.data != nullptr && *this->data == *rhs.data));
        return result;
    }

    bool ComponentData::operator !=(const ComponentData& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentData::ComponentData(const ComponentData& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentData& ComponentData::operator =(const ComponentData& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->data = rhs.data;
        return *this;
    }

    std::string ComponentData::get_type() const
    {
        return ComponentData::TYPE;
    }

    void ComponentData::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentData>(this);
    }

    void ComponentData::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentData>(this);
    }

    intrusive_ptr<ComponentBase> ComponentData::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentData>(id);
    }

    void ComponentData::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(data, "data");
    }

    void ComponentData::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(data, "data");
    }

    void ComponentData::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(data, "data");
    }

    void ComponentData::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(data, "data");
    }

} //namespace mg
