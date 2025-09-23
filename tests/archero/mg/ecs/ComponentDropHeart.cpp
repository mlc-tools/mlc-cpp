#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentDropHeart.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentDropHeart::TYPE("ComponentDropHeart");

    ComponentDropHeart::ComponentDropHeart()
    {

    }

    ComponentDropHeart::~ComponentDropHeart()
    {
    }

    bool ComponentDropHeart::operator ==(const ComponentDropHeart& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        return result;
    }

    bool ComponentDropHeart::operator !=(const ComponentDropHeart& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentDropHeart::ComponentDropHeart(const ComponentDropHeart& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentDropHeart& ComponentDropHeart::operator =(const ComponentDropHeart& rhs)
    {
        this->ComponentBase::operator=(rhs);
        return *this;
    }

    std::string ComponentDropHeart::get_type() const
    {
        return ComponentDropHeart::TYPE;
    }

    void ComponentDropHeart::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentDropHeart>(this);
    }

    void ComponentDropHeart::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentDropHeart>(this);
    }

    intrusive_ptr<ComponentBase> ComponentDropHeart::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentDropHeart>(id);
    }

    void ComponentDropHeart::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
    }

    void ComponentDropHeart::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
    }

    void ComponentDropHeart::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
    }

    void ComponentDropHeart::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
    }

} //namespace mg
