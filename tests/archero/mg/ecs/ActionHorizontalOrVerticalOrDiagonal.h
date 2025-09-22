#ifndef __mg_ActionHorizontalOrVerticalOrDiagonal_h__
#define __mg_ActionHorizontalOrVerticalOrDiagonal_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ActionMoveToCenterHorizontalOrVertical.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class ActionHorizontalOrVerticalOrDiagonal : public ActionMoveToCenterHorizontalOrVertical
    {
    public:
        ActionHorizontalOrVerticalOrDiagonal();
        virtual ~ActionHorizontalOrVerticalOrDiagonal();
        virtual Vector choose_direction() override;
        bool operator ==(const ActionHorizontalOrVerticalOrDiagonal& rhs) const;
        bool operator !=(const ActionHorizontalOrVerticalOrDiagonal& rhs) const;
        ActionHorizontalOrVerticalOrDiagonal(const ActionHorizontalOrVerticalOrDiagonal& rhs);
        const ActionHorizontalOrVerticalOrDiagonal& operator =(const ActionHorizontalOrVerticalOrDiagonal& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionHorizontalOrVerticalOrDiagonal_h__
