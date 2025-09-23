#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "ComponentHealth.h"
#include "ComponentSide.h"
#include "ComponentVampire.h"
#include "ModelEcsBase.h"
#include "Side.h"
#include "SystemVampire.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemVampire::TYPE("SystemVampire");

    SystemVampire::SystemVampire()
    : _reference_counter(1)
    {

    }

    SystemVampire::~SystemVampire()
    {
    }

    void SystemVampire::on_kill(ModelEcsBase* model, int killer_id, int target_id)
    {
        auto component = model->get<ComponentVampire>(killer_id);
        if(!component)
        {
            return;
        }

        auto side_killer = model->get<ComponentSide>(killer_id);
        auto side_target = model->get<ComponentSide>(target_id);
        if(!side_killer || !side_target || !side_killer->is_enemy(side_target->side))
        {
            return;
        }

        if(random_float() < component->chance)
        {
            auto health = model->get<ComponentHealth>(killer_id);
            if(health && health->get_current() < health->max)
            {
                auto value = health->max * component->heal_percent;
                value = std::min(value, health->max - health->get_current());
                health->set_current(health->get_current() + value);
                model->event_healing[health->id].notify(value);
            }
        }
    }

    void SystemVampire::retain()
    {
        ++this->_reference_counter;
    }

    int SystemVampire::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemVampire::operator ==(const SystemVampire& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemVampire::operator !=(const SystemVampire& rhs) const
    {
        return !(*this == rhs);
    }

    SystemVampire::SystemVampire(const SystemVampire& rhs)
    {
        this->operator=(rhs);
    }

    const SystemVampire& SystemVampire::operator =(const SystemVampire& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemVampire::get_type() const
    {
        return SystemVampire::TYPE;
    }

    void SystemVampire::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemVampire::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemVampire::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemVampire::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
