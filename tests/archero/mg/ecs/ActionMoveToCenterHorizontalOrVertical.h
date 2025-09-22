#ifndef __mg_ActionMoveToCenterHorizontalOrVertical_h__
#define __mg_ActionMoveToCenterHorizontalOrVertical_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ActionMoveDirection.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class ActionMoveToCenterHorizontalOrVertical : public ActionMoveDirection
    {
    public:
        ActionMoveToCenterHorizontalOrVertical();
        virtual ~ActionMoveToCenterHorizontalOrVertical();
        virtual Vector choose_direction() override;
        bool operator ==(const ActionMoveToCenterHorizontalOrVertical& rhs) const;
        bool operator !=(const ActionMoveToCenterHorizontalOrVertical& rhs) const;
        ActionMoveToCenterHorizontalOrVertical(const ActionMoveToCenterHorizontalOrVertical& rhs);
        const ActionMoveToCenterHorizontalOrVertical& operator =(const ActionMoveToCenterHorizontalOrVertical& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionMoveToCenterHorizontalOrVertical_h__
