#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentPush.h"
#include "ComponentStats.h"
#include "ModelEcsBase.h"
#include "SystemPush.h"
#include "SystemResolveCollisions.h"
#include "Transform.h"
#include "UnitStat.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemPush::TYPE("SystemPush");

    SystemPush::SystemPush()
    : _reference_counter(1)
    {

    }

    SystemPush::~SystemPush()
    {
    }

    void SystemPush::update(ModelEcsBase* model, float dt)
    {
        model->each<ComponentPush, Transform, ComponentStats>(
        [&](auto& push, auto& transform, auto& stats)
        {
            push->duration -= dt;
            auto resist = stats->get(model, UnitStat::push_target_resist);
            if(resist >= 1)
            {
                return;
            }
            auto offset = push->direction * (dt * push->speed) * (1-resist);
            auto new_pos =  transform->position + offset;
            auto can = SystemResolveCollisions::can_move(model, transform->id, new_pos, stats->get(model, UnitStat::size));
            if(can)
            {
                transform->set(new_pos);
            }
        });
    }

    void SystemPush::clean(ModelEcsBase* model)
    {
        for(int __index__ = 0; __index__ < model->components_push.size(); ++__index__)
        {
            auto& push = model->components_push.at(__index__);
            if(push->duration <= 0 )
            {
                auto __size__ = model->components_push.size();
                model->remove(push);
                if(__size__ != model->components_push.size()) { --__index__; }
            }
        }
    }

    void SystemPush::retain()
    {
        ++this->_reference_counter;
    }

    int SystemPush::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemPush::operator ==(const SystemPush& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemPush::operator !=(const SystemPush& rhs) const
    {
        return !(*this == rhs);
    }

    SystemPush::SystemPush(const SystemPush& rhs)
    {
        this->operator=(rhs);
    }

    const SystemPush& SystemPush::operator =(const SystemPush& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemPush::get_type() const
    {
        return SystemPush::TYPE;
    }

    void SystemPush::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemPush::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemPush::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemPush::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
