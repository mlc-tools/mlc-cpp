#ifndef __mg_ActionShoot_h__
#define __mg_ActionShoot_h__

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

    class ActionShoot : public ActionBase
    {
    public:
        ActionShoot();
        virtual ~ActionShoot();
        virtual void on_start() override;
        virtual void on_finish() override;
        virtual void on_update(float dt) override;
        virtual void shoot();
        bool operator ==(const ActionShoot& rhs) const;
        bool operator !=(const ActionShoot& rhs) const;
        ActionShoot(const ActionShoot& rhs);
        const ActionShoot& operator =(const ActionShoot& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::string animation;
        std::string marker_id;
        std::string bullet_name;
        bool was_shoot;
        float time_shoot;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionShoot_h__
