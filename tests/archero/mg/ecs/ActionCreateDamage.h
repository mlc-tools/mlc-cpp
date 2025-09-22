#ifndef __mg_ActionCreateDamage_h__
#define __mg_ActionCreateDamage_h__

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

    class ActionCreateDamage : public ActionBase
    {
    public:
        ActionCreateDamage();
        virtual ~ActionCreateDamage();
        virtual void on_start() override;
        virtual void on_finish() override;
        bool operator ==(const ActionCreateDamage& rhs) const;
        bool operator !=(const ActionCreateDamage& rhs) const;
        ActionCreateDamage(const ActionCreateDamage& rhs);
        const ActionCreateDamage& operator =(const ActionCreateDamage& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionCreateDamage_h__
