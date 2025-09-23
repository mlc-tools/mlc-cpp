#ifndef __mg_ActionCreateMarkerNearHero_h__
#define __mg_ActionCreateMarkerNearHero_h__

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


    class ActionCreateMarkerNearHero : public ActionBase
    {
    public:
        ActionCreateMarkerNearHero();
        virtual ~ActionCreateMarkerNearHero();
        virtual void on_start() override;
        bool operator ==(const ActionCreateMarkerNearHero& rhs) const;
        bool operator !=(const ActionCreateMarkerNearHero& rhs) const;
        ActionCreateMarkerNearHero(const ActionCreateMarkerNearHero& rhs);
        const ActionCreateMarkerNearHero& operator =(const ActionCreateMarkerNearHero& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::string marker;
        std::string marker_id;
        float radius;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionCreateMarkerNearHero_h__
