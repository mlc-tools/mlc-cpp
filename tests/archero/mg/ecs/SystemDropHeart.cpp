#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "BuilderEntity.h"
#include "ComponentDropHeart.h"
#include "ComponentHealth.h"
#include "ComponentHeart.h"
#include "ComponentHeartAddStats.h"
#include "ComponentHeartAddStatsDamage.h"
#include "ComponentHeartAddStatsHp.h"
#include "ComponentStats.h"
#include "DataUnit.h"
#include "ModelEcsBase.h"
#include "Side.h"
#include "SystemDropHeart.h"
#include "Transform.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemDropHeart::TYPE("SystemDropHeart");

    SystemDropHeart::SystemDropHeart()
    : _reference_counter(1)
    {

    }

    SystemDropHeart::~SystemDropHeart()
    {
    }

    void SystemDropHeart::update(ModelEcsBase* model, float dt)
    {
        model->each_if<ComponentDropHeart,ComponentHealth>(
        [&](auto& drop, auto& component_health)
        {
            return component_health->get_current() <= 0;
        },
        [&](auto& drop, auto& component_health)
        {
            auto chance = 0.1f;
            if(random_float() < chance)
            {
                auto transform = model->get<Transform>(drop->id);
                auto data = DataStorage::shared().get<DataUnit>("entity_heart");
                auto id = BuilderEntity(data).set_side(mg::Side::ally).set_position(transform->position).build(model);
                model->event_create_entity.notify(id, transform->position);
            }
        });

        auto player_transform = model->get<Transform>(model->player_id);
        auto dist = 100 * 100;
        model->each_if<ComponentHeart,Transform>(
        [&](auto& heart, auto& transform)
        {
            return transform->position.get_distance_sq(player_transform->position) < dist;
        },
        [&](auto& heart, auto& transform)
        {
            auto health = model->get<ComponentHealth>(model->player_id);
            auto add = health->max * 0.25f;
            add = std::min(add, health->max - health->get_current());
            health->set_current(health->get_current() + add);

            this->check_lucky_heart(model, model->get<ComponentHeartAddStatsHp>(model->player_id));
            this->check_lucky_heart(model, model->get<ComponentHeartAddStatsDamage>(model->player_id));

            heart->clean = true;
        });
    }

    void SystemDropHeart::check_lucky_heart(ModelEcsBase* model, ComponentHeartAddStats* component)
    {
        auto player_stats = model->get<ComponentStats>(model->player_id);
        if(component && random_float() < component->percent)
        {
            player_stats->add_upgrade(component->stat_upgrade);
        }
    }

    void SystemDropHeart::clean(ModelEcsBase* model)
    {
        for(int __index__ = 0; __index__ < model->components_heart.size(); ++__index__)
        {
            auto& component = model->components_heart.at(__index__);
            if(component->clean == true )
            {
                auto __size__ = model->components_heart.size();
                model->remove_entity(component->id);
                if(__size__ != model->components_heart.size()) { --__index__; }
            }
        }
    }

    void SystemDropHeart::retain()
    {
        ++this->_reference_counter;
    }

    int SystemDropHeart::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemDropHeart::operator ==(const SystemDropHeart& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemDropHeart::operator !=(const SystemDropHeart& rhs) const
    {
        return !(*this == rhs);
    }

    SystemDropHeart::SystemDropHeart(const SystemDropHeart& rhs)
    {
        this->operator=(rhs);
    }

    const SystemDropHeart& SystemDropHeart::operator =(const SystemDropHeart& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemDropHeart::get_type() const
    {
        return SystemDropHeart::TYPE;
    }

    void SystemDropHeart::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemDropHeart::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemDropHeart::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemDropHeart::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
