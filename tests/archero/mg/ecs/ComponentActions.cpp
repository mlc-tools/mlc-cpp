#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "ActionBase.h"
#include "ComponentActions.h"
#include "ComponentBase.h"
#include "Dictionary.h"
#include "ModelEcsBase.h"
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentActions::TYPE("ComponentActions");

    ComponentActions::ComponentActions()
    : initialized(false)
    , actions()
    , map_actions()
    , current_action(nullptr)
    , first_action("")
    , next_action("")
    , dictionary(nullptr)
    {

    }

    ComponentActions::~ComponentActions()
    {
    }

    intrusive_ptr<ActionBase> ComponentActions::get_action(const std::string& name)
    {
        return this->map_actions.at(name);
    }

    void ComponentActions::initialize(ModelEcsBase* model, int entity_id)
    {
        assert(!this->initialized);
        this->initialized = true;

        this->dictionary = make_intrusive<Dictionary>();

        assert(this->map_actions.size() == 0);
        for(auto& action : this->actions)
        {
            action->model = model;
            action->entity_id = entity_id;
            action->common_dictionary = this->dictionary;

            if(action->random_duration)
            {
                action->duration += random_float() * action->random_duration;
            }

            this->map_actions[action->name] = action;
        }
        this->first_action = this->actions[0]->name;
    }

    bool ComponentActions::operator ==(const ComponentActions& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->initialized == rhs.initialized;
        result = result && this->actions == rhs.actions;
        result = result && this->map_actions == rhs.map_actions;
        result = result && ((this->current_action == rhs.current_action) || (this->current_action != nullptr && rhs.current_action != nullptr && *this->current_action == *rhs.current_action));
        result = result && this->first_action == rhs.first_action;
        result = result && this->next_action == rhs.next_action;
        result = result && ((this->dictionary == rhs.dictionary) || (this->dictionary != nullptr && rhs.dictionary != nullptr && *this->dictionary == *rhs.dictionary));
        return result;
    }

    bool ComponentActions::operator !=(const ComponentActions& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentActions::ComponentActions(const ComponentActions& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentActions& ComponentActions::operator =(const ComponentActions& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->initialized = rhs.initialized;
        this->actions = rhs.actions;
        this->map_actions = rhs.map_actions;
        this->current_action = rhs.current_action;
        this->first_action = rhs.first_action;
        this->next_action = rhs.next_action;
        this->dictionary = rhs.dictionary;
        return *this;
    }

    std::string ComponentActions::get_type() const
    {
        return ComponentActions::TYPE;
    }

    void ComponentActions::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentActions>(this);
    }

    void ComponentActions::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentActions>(this);
    }

    intrusive_ptr<ComponentBase> ComponentActions::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentActions>(id);
    }

    void ComponentActions::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(initialized, "initialized", bool(false));
        serializer.serialize(actions, "actions");
        serializer.serialize(map_actions, "map_actions");
        serializer.serialize(current_action, "current_action");
        serializer.serialize(first_action, "first_action", std::string(""));
        serializer.serialize(next_action, "next_action", std::string(""));
        serializer.serialize(dictionary, "dictionary");
    }

    void ComponentActions::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(initialized, "initialized", bool(false));
        deserializer.deserialize(actions, "actions");
        deserializer.deserialize(map_actions, "map_actions");
        deserializer.deserialize(current_action, "current_action");
        deserializer.deserialize(first_action, "first_action", std::string(""));
        deserializer.deserialize(next_action, "next_action", std::string(""));
        deserializer.deserialize(dictionary, "dictionary");
    }

    void ComponentActions::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(initialized, "initialized", bool(false));
        serializer.serialize(actions, "actions");
        serializer.serialize(map_actions, "map_actions");
        serializer.serialize(current_action, "current_action");
        serializer.serialize(first_action, "first_action", std::string(""));
        serializer.serialize(next_action, "next_action", std::string(""));
        serializer.serialize(dictionary, "dictionary");
    }

    void ComponentActions::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(initialized, "initialized", bool(false));
        deserializer.deserialize(actions, "actions");
        deserializer.deserialize(map_actions, "map_actions");
        deserializer.deserialize(current_action, "current_action");
        deserializer.deserialize(first_action, "first_action", std::string(""));
        deserializer.deserialize(next_action, "next_action", std::string(""));
        deserializer.deserialize(dictionary, "dictionary");
    }

} //namespace mg
