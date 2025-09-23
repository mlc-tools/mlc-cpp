#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ComponentBase.h"
#include "ComponentSide.h"
#include "ModelEcsBase.h"
#include "Side.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSide::TYPE("ComponentSide");

    ComponentSide::ComponentSide()
    : side()
    {

    }

    ComponentSide::~ComponentSide()
    {
    }

    bool ComponentSide::is_enemy(const Side& side) const
    {
        switch(this->side)
        {
            case Side::neutral: return false;
            case Side::ally: return side == Side::enemy;
            case Side::enemy: return side == Side::ally;
        }
        return false;
    }

    bool ComponentSide::operator ==(const ComponentSide& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->side == rhs.side;
        return result;
    }

    bool ComponentSide::operator !=(const ComponentSide& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSide::ComponentSide(const ComponentSide& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSide& ComponentSide::operator =(const ComponentSide& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->side = rhs.side;
        return *this;
    }

    std::string ComponentSide::get_type() const
    {
        return ComponentSide::TYPE;
    }

    void ComponentSide::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSide>(this);
    }

    void ComponentSide::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSide>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSide::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSide>(id);
    }

    void ComponentSide::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(side, "side");
    }

    void ComponentSide::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(side, "side");
    }

    void ComponentSide::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(side, "side");
    }

    void ComponentSide::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(side, "side");
    }

} //namespace mg
