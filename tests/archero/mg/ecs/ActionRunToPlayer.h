#ifndef __mg_ActionRunToPlayer_h__
#define __mg_ActionRunToPlayer_h__

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


    class ActionRunToPlayer : public ActionBase
    {
    public:
        ActionRunToPlayer();
        virtual ~ActionRunToPlayer();
        virtual void on_start() override;
        virtual void on_update(float dt) override;
        virtual void on_break() override;
        virtual void on_finish() override;
        void finalize();
        bool operator ==(const ActionRunToPlayer& rhs) const;
        bool operator !=(const ActionRunToPlayer& rhs) const;
        ActionRunToPlayer(const ActionRunToPlayer& rhs);
        const ActionRunToPlayer& operator =(const ActionRunToPlayer& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::string animation;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionRunToPlayer_h__
