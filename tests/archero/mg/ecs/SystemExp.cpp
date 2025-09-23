#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../data/DataLadderLevels.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "BuilderEntity.h"
#include "ComponentExp.h"
#include "ComponentExpDrop.h"
#include "ComponentHealth.h"
#include "ComponentLevelUp.h"
#include "DataUnit.h"
#include "ModelEcsBase.h"
#include "Side.h"
#include "SystemExp.h"
#include "Transform.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemExp::TYPE("SystemExp");

    SystemExp::SystemExp()
    : _reference_counter(1)
    {

    }

    SystemExp::~SystemExp()
    {
    }

    void SystemExp::update(ModelEcsBase* model, float dt)
    {
        this->create_exp(model);

        auto player_transform = model->get<Transform>(model->player_id);
        auto dist = 100 * 100;
        model->each<ComponentExp,Transform>(
        [&](auto& exp, auto& transform)
        {
            if(transform->position.get_distance_sq(player_transform->position) < dist)
            {
                auto level = model->get<ComponentLevelUp>(model->player_id);
                level->exp_current += exp->count;
                model->event_hero_exp_changed.notify();

                exp->clean = true;
            }
        });

        auto ladder = DataStorage::shared().get<DataLadderLevels>("hero_exp");
        for(auto& level_up : model->components_level_up)
        {
            auto level = ladder->get_level(level_up->exp_current);
            if(level_up->level != level)
            {
                level_up->level += 1;
                model->event_hero_level_up.notify();
            }
        }
    }

    void SystemExp::clean(ModelEcsBase* model)
    {
        for(int __index__ = 0; __index__ < model->components_exp.size(); ++__index__)
        {
            auto& component = model->components_exp.at(__index__);
            if(component->clean == true )
            {
                auto __size__ = model->components_exp.size();
                model->remove_entity(component->id);
                if(__size__ != model->components_exp.size()) { --__index__; }
            }
        }
    }

    void SystemExp::create_exp(ModelEcsBase* model)
    {
        model->each_if<ComponentExpDrop, ComponentHealth>(
        [&](auto& exp_drop, auto& health)
        {
            return health->get_current() <= 0;
        },
        [&](auto& exp_drop, auto& health)
        {
            auto transform = model->get<Transform>(exp_drop->id);
            auto data = DataStorage::shared().get<DataUnit>("entity_exp");
            auto id = BuilderEntity(data).set_side(mg::Side::ally).set_position(transform->position).build(model);
            model->event_create_entity.notify(id, transform->position);

            auto component_exp = model->get<ComponentExp>(id);
            component_exp->count = exp_drop->count;
        });
    }

    void SystemExp::retain()
    {
        ++this->_reference_counter;
    }

    int SystemExp::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemExp::operator ==(const SystemExp& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemExp::operator !=(const SystemExp& rhs) const
    {
        return !(*this == rhs);
    }

    SystemExp::SystemExp(const SystemExp& rhs)
    {
        this->operator=(rhs);
    }

    const SystemExp& SystemExp::operator =(const SystemExp& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemExp::get_type() const
    {
        return SystemExp::TYPE;
    }

    void SystemExp::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemExp::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemExp::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemExp::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
