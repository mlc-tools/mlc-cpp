#ifndef __mg_ActionMeleeAttack_h__
#define __mg_ActionMeleeAttack_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ActionBase.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class ActionMeleeAttack : public ActionBase
    {
    public:
        ActionMeleeAttack();
        virtual ~ActionMeleeAttack();
        virtual void on_start() override;
        virtual void on_finish() override;
        bool operator ==(const ActionMeleeAttack& rhs) const;
        bool operator !=(const ActionMeleeAttack& rhs) const;
        ActionMeleeAttack(const ActionMeleeAttack& rhs);
        const ActionMeleeAttack& operator =(const ActionMeleeAttack& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionMeleeAttack_h__
