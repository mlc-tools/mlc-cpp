#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentGate.h"
#include "ModelEcsBase.h"
#include "SystemGate.h"
#include "Transform.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemGate::TYPE("SystemGate");

    SystemGate::SystemGate()
    : _reference_counter(1)
    {

    }

    SystemGate::~SystemGate()
    {
    }

    void SystemGate::update(ModelEcsBase* model, float dt)
    {
        model->each<ComponentGate, Transform>(
        [&](auto& gate, auto& transform)
        {
            auto player_transform = model->get<Transform>(model->player_id);
            auto dist = player_transform->position.get_distance(transform->position);
            if(dist < gate->radius)
            {
                model->event_wave_next.notify();
                gate->clean = true;
            }
        });
        for(int __index__ = 0; __index__ < model->components_gate.size(); ++__index__)
        {
            auto& gate = model->components_gate.at(__index__);
            if(gate->clean )
            {
                auto __size__ = model->components_gate.size();
                model->remove_entity(gate->id);
                if(__size__ != model->components_gate.size()) { --__index__; }
            }
        }
    }

    void SystemGate::retain()
    {
        ++this->_reference_counter;
    }

    int SystemGate::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemGate::operator ==(const SystemGate& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemGate::operator !=(const SystemGate& rhs) const
    {
        return !(*this == rhs);
    }

    SystemGate::SystemGate(const SystemGate& rhs)
    {
        this->operator=(rhs);
    }

    const SystemGate& SystemGate::operator =(const SystemGate& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemGate::get_type() const
    {
        return SystemGate::TYPE;
    }

    void SystemGate::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemGate::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemGate::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemGate::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
