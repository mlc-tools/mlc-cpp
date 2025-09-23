#ifndef __mg_ActionWait_h__
#define __mg_ActionWait_h__

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


    class ActionWait : public ActionBase
    {
    public:
        ActionWait();
        virtual ~ActionWait();
        virtual void on_start() override;
        virtual void on_finish() override;
        bool operator ==(const ActionWait& rhs) const;
        bool operator !=(const ActionWait& rhs) const;
        ActionWait(const ActionWait& rhs);
        const ActionWait& operator =(const ActionWait& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::string animation;
        bool animation_loop;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionWait_h__
