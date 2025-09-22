#ifndef __mg_ActionShootByMoveDirection_h__
#define __mg_ActionShootByMoveDirection_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ActionShoot.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class ActionShootByMoveDirection : public ActionShoot
    {
    public:
        ActionShootByMoveDirection();
        virtual ~ActionShootByMoveDirection();
        virtual void shoot() override;
        bool operator ==(const ActionShootByMoveDirection& rhs) const;
        bool operator !=(const ActionShootByMoveDirection& rhs) const;
        ActionShootByMoveDirection(const ActionShootByMoveDirection& rhs);
        const ActionShootByMoveDirection& operator =(const ActionShootByMoveDirection& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionShootByMoveDirection_h__
