#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentSpiritBasePoints.h"
#include "ModelEcsBase.h"
#include "Vector.h"
#include <map>
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentSpiritBasePoints::TYPE("ComponentSpiritBasePoints");

    ComponentSpiritBasePoints::ComponentSpiritBasePoints()
    : points()
    {

    }

    ComponentSpiritBasePoints::~ComponentSpiritBasePoints()
    {
    }

    bool ComponentSpiritBasePoints::operator ==(const ComponentSpiritBasePoints& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->points == rhs.points;
        return result;
    }

    bool ComponentSpiritBasePoints::operator !=(const ComponentSpiritBasePoints& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentSpiritBasePoints::ComponentSpiritBasePoints(const ComponentSpiritBasePoints& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentSpiritBasePoints& ComponentSpiritBasePoints::operator =(const ComponentSpiritBasePoints& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->points = rhs.points;
        return *this;
    }

    std::string ComponentSpiritBasePoints::get_type() const
    {
        return ComponentSpiritBasePoints::TYPE;
    }

    void ComponentSpiritBasePoints::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentSpiritBasePoints>(this);
    }

    void ComponentSpiritBasePoints::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentSpiritBasePoints>(this);
    }

    intrusive_ptr<ComponentBase> ComponentSpiritBasePoints::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentSpiritBasePoints>(id);
    }

    void ComponentSpiritBasePoints::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(points, "points");
    }

    void ComponentSpiritBasePoints::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(points, "points");
    }

    void ComponentSpiritBasePoints::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(points, "points");
    }

    void ComponentSpiritBasePoints::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(points, "points");
    }

} //namespace mg
