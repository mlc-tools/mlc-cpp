#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "ComponentStun.h"
#include "ModelEcsBase.h"
#include "SystemStun.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemStun::TYPE("SystemStun");

    SystemStun::SystemStun()
    : _reference_counter(1)
    {

    }

    SystemStun::~SystemStun()
    {
    }

    void SystemStun::update(ModelEcsBase* model, float dt)
    {
        model->each<ComponentStun>(
        [&](auto& stun)
        {
            if(stun->resist_timer > 0)
            {
                stun->resist_timer -= dt;
            }
            if(!stun->activated)
            {
                return;
            }
            stun->timer -= dt;
            if(stun->timer <= 0)
            {
                stun->activated = false;
                stun->timer = 0;
                stun->resist_timer = 5;
                model->event_stun_deactivated[stun->id].notify();
            }
        });
    }

    void SystemStun::apply(ModelEcsBase* model, int target_id, float duration)
    {
        auto stun = model->get<ComponentStun>(target_id);
        if(!stun)
        {
            stun = make_intrusive<ComponentStun>();
            model->add(stun, target_id);
        }
        if(stun->resist_timer > 0 || stun->activated)
        {
            return;
        }

        stun->activated = true;
        stun->timer = std::max(stun->timer, duration);
        model->event_stun_activated[target_id].notify();
    }

    void SystemStun::retain()
    {
        ++this->_reference_counter;
    }

    int SystemStun::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemStun::operator ==(const SystemStun& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemStun::operator !=(const SystemStun& rhs) const
    {
        return !(*this == rhs);
    }

    SystemStun::SystemStun(const SystemStun& rhs)
    {
        this->operator=(rhs);
    }

    const SystemStun& SystemStun::operator =(const SystemStun& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemStun::get_type() const
    {
        return SystemStun::TYPE;
    }

    void SystemStun::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemStun::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemStun::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemStun::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
