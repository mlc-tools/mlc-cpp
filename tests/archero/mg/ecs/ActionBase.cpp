#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ActionBase.h"
#include "ComponentSpineInfo.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionBase::TYPE("ActionBase");

    ActionBase::ActionBase()
    : model(nullptr)
    , entity_id(0)
    , duration(0)
    , random_duration(0)
    , timer(0)
    , name("")
    , common_dictionary(nullptr)
    , next_action_on_break("")
    , next_action_on_finish("")
    , next_action("")
    , _finished(false)
    , _breaked(false)
    , switch_to_action("")
    , _reference_counter(1)
    {

    }

    ActionBase::~ActionBase()
    {
    }

    bool ActionBase::is_finished() const
    {
        return this->_finished;
    }

    bool ActionBase::is_breaked() const
    {
        return this->_breaked;
    }

    void ActionBase::start()
    {
        this->_finished = false;
        this->_breaked = false;
        this->timer = 0;
        this->on_start();
    }

    void ActionBase::update(float dt)
    {
        if(duration > 0)
        {
            this->timer += dt;
            if(this->timer >= this->duration)
            {
                this->_finished = true;
                this->set_next_action(this->next_action_on_finish);
            }
        }
        this->on_update(dt);
    }

    void ActionBase::break_()
    {
        this->on_break();
    }

    void ActionBase::finish()
    {
        this->on_finish();
    }

    void ActionBase::on_start()
    {

    }

    void ActionBase::on_update(float dt)
    {

    }

    void ActionBase::on_break()
    {

    }

    void ActionBase::on_finish()
    {

    }

    void ActionBase::set_next_action(const std::string& action)
    {
        if(string_size(action) > 0)
        {
            this->switch_to_action = action;
        }
        else
        {
            this->switch_to_action = next_action;
        }
        assert(string_size(this->switch_to_action) > 0);
    }

    void ActionBase::check_duration(const std::string& animation)
    {
        if(!animation.empty())
        {
            if(this->duration == 0)
            {
                this->duration = model->get<ComponentSpineInfo>(this->entity_id)->get_animation_duration(animation);
            }
        }
    }

    void ActionBase::retain()
    {
        ++this->_reference_counter;
    }

    int ActionBase::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool ActionBase::operator ==(const ActionBase& rhs) const
    {
        bool result = true;
        result = result && ((this->model == rhs.model) || (this->model != nullptr && rhs.model != nullptr && *this->model == *rhs.model));
        result = result && this->entity_id == rhs.entity_id;
        result = result && this->duration == rhs.duration;
        result = result && this->random_duration == rhs.random_duration;
        result = result && this->timer == rhs.timer;
        result = result && this->name == rhs.name;
        result = result && ((this->common_dictionary == rhs.common_dictionary) || (this->common_dictionary != nullptr && rhs.common_dictionary != nullptr && *this->common_dictionary == *rhs.common_dictionary));
        result = result && this->next_action_on_break == rhs.next_action_on_break;
        result = result && this->next_action_on_finish == rhs.next_action_on_finish;
        result = result && this->next_action == rhs.next_action;
        result = result && this->_finished == rhs._finished;
        result = result && this->_breaked == rhs._breaked;
        result = result && this->switch_to_action == rhs.switch_to_action;
        return result;
    }

    bool ActionBase::operator !=(const ActionBase& rhs) const
    {
        return !(*this == rhs);
    }

    ActionBase::ActionBase(const ActionBase& rhs)
    {
        this->operator=(rhs);
    }

    const ActionBase& ActionBase::operator =(const ActionBase& rhs)
    {
        this->model = rhs.model;
        this->entity_id = rhs.entity_id;
        this->duration = rhs.duration;
        this->random_duration = rhs.random_duration;
        this->timer = rhs.timer;
        this->name = rhs.name;
        this->common_dictionary = rhs.common_dictionary;
        this->next_action_on_break = rhs.next_action_on_break;
        this->next_action_on_finish = rhs.next_action_on_finish;
        this->next_action = rhs.next_action;
        this->_finished = rhs._finished;
        this->_breaked = rhs._breaked;
        this->switch_to_action = rhs.switch_to_action;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string ActionBase::get_type() const
    {
        return ActionBase::TYPE;
    }

    void ActionBase::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(model, "model");
        serializer.serialize(entity_id, "entity_id", int(0));
        serializer.serialize(duration, "duration", float(0));
        serializer.serialize(random_duration, "random_duration", float(0));
        serializer.serialize(timer, "timer", float(0));
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(common_dictionary, "common_dictionary");
        serializer.serialize(next_action_on_break, "next_action_on_break", std::string(""));
        serializer.serialize(next_action_on_finish, "next_action_on_finish", std::string(""));
        serializer.serialize(next_action, "next_action", std::string(""));
        serializer.serialize(_finished, "_finished", bool(false));
        serializer.serialize(_breaked, "_breaked", bool(false));
        serializer.serialize(switch_to_action, "switch_to_action", std::string(""));
    }

    void ActionBase::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(model, "model");
        deserializer.deserialize(entity_id, "entity_id", int(0));
        deserializer.deserialize(duration, "duration", float(0));
        deserializer.deserialize(random_duration, "random_duration", float(0));
        deserializer.deserialize(timer, "timer", float(0));
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(common_dictionary, "common_dictionary");
        deserializer.deserialize(next_action_on_break, "next_action_on_break", std::string(""));
        deserializer.deserialize(next_action_on_finish, "next_action_on_finish", std::string(""));
        deserializer.deserialize(next_action, "next_action", std::string(""));
        deserializer.deserialize(_finished, "_finished", bool(false));
        deserializer.deserialize(_breaked, "_breaked", bool(false));
        deserializer.deserialize(switch_to_action, "switch_to_action", std::string(""));
    }

    void ActionBase::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(model, "model");
        serializer.serialize(entity_id, "entity_id", int(0));
        serializer.serialize(duration, "duration", float(0));
        serializer.serialize(random_duration, "random_duration", float(0));
        serializer.serialize(timer, "timer", float(0));
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(common_dictionary, "common_dictionary");
        serializer.serialize(next_action_on_break, "next_action_on_break", std::string(""));
        serializer.serialize(next_action_on_finish, "next_action_on_finish", std::string(""));
        serializer.serialize(next_action, "next_action", std::string(""));
        serializer.serialize(_finished, "_finished", bool(false));
        serializer.serialize(_breaked, "_breaked", bool(false));
        serializer.serialize(switch_to_action, "switch_to_action", std::string(""));
    }

    void ActionBase::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(model, "model");
        deserializer.deserialize(entity_id, "entity_id", int(0));
        deserializer.deserialize(duration, "duration", float(0));
        deserializer.deserialize(random_duration, "random_duration", float(0));
        deserializer.deserialize(timer, "timer", float(0));
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(common_dictionary, "common_dictionary");
        deserializer.deserialize(next_action_on_break, "next_action_on_break", std::string(""));
        deserializer.deserialize(next_action_on_finish, "next_action_on_finish", std::string(""));
        deserializer.deserialize(next_action, "next_action", std::string(""));
        deserializer.deserialize(_finished, "_finished", bool(false));
        deserializer.deserialize(_breaked, "_breaked", bool(false));
        deserializer.deserialize(switch_to_action, "switch_to_action", std::string(""));
    }

} //namespace mg
