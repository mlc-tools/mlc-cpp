#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionHorizontalOrVerticalOrDiagonal.h"
#include "ActionMoveToCenterHorizontalOrVertical.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionHorizontalOrVerticalOrDiagonal::TYPE("ActionHorizontalOrVerticalOrDiagonal");

    ActionHorizontalOrVerticalOrDiagonal::ActionHorizontalOrVerticalOrDiagonal()
    {

    }

    ActionHorizontalOrVerticalOrDiagonal::~ActionHorizontalOrVerticalOrDiagonal()
    {
    }

    Vector ActionHorizontalOrVerticalOrDiagonal::choose_direction()
    {
        Vector direction;
        direction.x = random_int(0, 3) - 1;
        direction.y = random_int(0, 3) - 1;
        direction.normalize();
        return direction;
    }

    bool ActionHorizontalOrVerticalOrDiagonal::operator ==(const ActionHorizontalOrVerticalOrDiagonal& rhs) const
    {
        bool result = this->ActionMoveToCenterHorizontalOrVertical::operator ==(rhs);
        return result;
    }

    bool ActionHorizontalOrVerticalOrDiagonal::operator !=(const ActionHorizontalOrVerticalOrDiagonal& rhs) const
    {
        return !(*this == rhs);
    }

    ActionHorizontalOrVerticalOrDiagonal::ActionHorizontalOrVerticalOrDiagonal(const ActionHorizontalOrVerticalOrDiagonal& rhs)
    {
        this->operator=(rhs);
    }

    const ActionHorizontalOrVerticalOrDiagonal& ActionHorizontalOrVerticalOrDiagonal::operator =(const ActionHorizontalOrVerticalOrDiagonal& rhs)
    {
        this->ActionMoveToCenterHorizontalOrVertical::operator=(rhs);
        return *this;
    }

    std::string ActionHorizontalOrVerticalOrDiagonal::get_type() const
    {
        return ActionHorizontalOrVerticalOrDiagonal::TYPE;
    }

    void ActionHorizontalOrVerticalOrDiagonal::serialize_xml(SerializerXml& serializer) const
    {
        ActionMoveToCenterHorizontalOrVertical::serialize_xml(serializer);
    }

    void ActionHorizontalOrVerticalOrDiagonal::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionMoveToCenterHorizontalOrVertical::deserialize_xml(deserializer);
    }

    void ActionHorizontalOrVerticalOrDiagonal::serialize_json(SerializerJson& serializer) const
    {
        ActionMoveToCenterHorizontalOrVertical::serialize_json(serializer);
    }

    void ActionHorizontalOrVerticalOrDiagonal::deserialize_json(DeserializerJson& deserializer)
    {
        ActionMoveToCenterHorizontalOrVertical::deserialize_json(deserializer);
    }

} //namespace mg
