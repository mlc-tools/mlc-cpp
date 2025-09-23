#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBase.h"
#include "ComponentHeart.h"
#include "ModelEcsBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentHeart::TYPE("ComponentHeart");

    ComponentHeart::ComponentHeart()
    : clean(false)
    {

    }

    ComponentHeart::~ComponentHeart()
    {
    }

    bool ComponentHeart::operator ==(const ComponentHeart& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->clean == rhs.clean;
        return result;
    }

    bool ComponentHeart::operator !=(const ComponentHeart& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentHeart::ComponentHeart(const ComponentHeart& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentHeart& ComponentHeart::operator =(const ComponentHeart& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->clean = rhs.clean;
        return *this;
    }

    std::string ComponentHeart::get_type() const
    {
        return ComponentHeart::TYPE;
    }

    void ComponentHeart::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentHeart>(this);
    }

    void ComponentHeart::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentHeart>(this);
    }

    intrusive_ptr<ComponentBase> ComponentHeart::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentHeart>(id);
    }

    void ComponentHeart::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(clean, "clean", bool(false));
    }

    void ComponentHeart::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(clean, "clean", bool(false));
    }

    void ComponentHeart::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(clean, "clean", bool(false));
    }

    void ComponentHeart::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(clean, "clean", bool(false));
    }

} //namespace mg
