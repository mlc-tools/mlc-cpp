#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentActions.h"
#include "ComponentStun.h"
#include "ModelEcsBase.h"
#include "SystemActions.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemActions::TYPE("SystemActions");

    SystemActions::SystemActions()
    : _reference_counter(1)
    {

    }

    SystemActions::~SystemActions()
    {
    }

    void SystemActions::update(ModelEcsBase* model, float dt)
    {
        for(auto component : model->components_actions)
        {
            if(!component->initialized)
            {
                component->initialize(model, component->id);
            }
            if(component->current_action == nullptr)
            {
                this->switch_to_next(model, component);
            }

            if(auto stun = model->get<ComponentStun>(component->id))
            {
                if(stun->activated)
                {
                    continue;
                }
            }

            component->current_action->update(dt);

            if(component->current_action->is_finished())
            {
                component->current_action->finish();
                component->next_action = component->current_action->switch_to_action;
                component->current_action = nullptr;
            }
            else if(component->current_action->is_breaked())
            {
                component->current_action->break_();
                component->next_action = component->current_action->switch_to_action;
                component->current_action = nullptr;
            }
        }
    }

    void SystemActions::switch_to_next(ModelEcsBase* model, ComponentActions* component)
    {
        assert(component->current_action == nullptr);

        if(string_size(component->next_action) == 0)
        {
            component->next_action = component->first_action;
        }
        component->current_action = component->get_action(component->next_action);
        component->current_action->start();
    }

    void SystemActions::retain()
    {
        ++this->_reference_counter;
    }

    int SystemActions::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemActions::operator ==(const SystemActions& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemActions::operator !=(const SystemActions& rhs) const
    {
        return !(*this == rhs);
    }

    SystemActions::SystemActions(const SystemActions& rhs)
    {
        this->operator=(rhs);
    }

    const SystemActions& SystemActions::operator =(const SystemActions& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemActions::get_type() const
    {
        return SystemActions::TYPE;
    }

    void SystemActions::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemActions::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemActions::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemActions::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
