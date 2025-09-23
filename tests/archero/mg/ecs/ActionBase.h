#ifndef __mg_ActionBase_h__
#define __mg_ActionBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Dictionary.h"
#include "ModelEcsBase.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class ActionBase
    {
    public:
        ActionBase();
        virtual ~ActionBase();
        bool is_finished() const;
        bool is_breaked() const;
        void start();
        void update(float dt);
        void break_();
        void finish();
    protected:
        virtual void on_start();
        virtual void on_update(float dt);
        virtual void on_break();
        virtual void on_finish();
    public:
        void set_next_action(const std::string& action);
        void check_duration(const std::string& animation);
        void retain();
        int release();
        bool operator ==(const ActionBase& rhs) const;
        bool operator !=(const ActionBase& rhs) const;
        ActionBase(const ActionBase& rhs);
        const ActionBase& operator =(const ActionBase& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

        intrusive_ptr<ModelEcsBase> model;
        int entity_id;
        float duration;
        float random_duration;
        float timer;
        std::string name;
        intrusive_ptr<Dictionary> common_dictionary;
    protected:
        std::string next_action_on_break;
        std::string next_action_on_finish;
        std::string next_action;
        bool _finished;
        bool _breaked;
    public:
        std::string switch_to_action;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ActionBase_h__
