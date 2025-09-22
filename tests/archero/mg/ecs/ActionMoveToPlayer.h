#ifndef __mg_ActionMoveToPlayer_h__
#define __mg_ActionMoveToPlayer_h__

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

    class ActionMoveToPlayer : public ActionBase
    {
    public:
        ActionMoveToPlayer();
        virtual ~ActionMoveToPlayer();
        virtual void on_start() override;
        virtual void on_update(float dt) override;
        virtual void on_break() override;
        virtual void on_finish() override;
        void finalize();
        bool operator ==(const ActionMoveToPlayer& rhs) const;
        bool operator !=(const ActionMoveToPlayer& rhs) const;
        ActionMoveToPlayer(const ActionMoveToPlayer& rhs);
        const ActionMoveToPlayer& operator =(const ActionMoveToPlayer& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionMoveToPlayer_h__
