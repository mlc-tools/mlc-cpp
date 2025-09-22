#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/ModelEquipmentSlot.h"
#include "../model/ModelUser.h"
#include "../systems/SystemEquipment.h"
#include "BuilderEntity.h"
#include "BulletType.h"
#include "ComponentBase.h"
#include "ComponentBody.h"
#include "ComponentBusy.h"
#include "ComponentData.h"
#include "ComponentHealth.h"
#include "ComponentRecharge.h"
#include "ComponentSide.h"
#include "ComponentStats.h"
#include "ComponentTargetable.h"
#include "ComponentUser.h"
#include "DataUnit.h"
#include "Freezing.h"
#include "ModelEcsBase.h"
#include "Side.h"
#include "Transform.h"
#include "UnitStat.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string BuilderEntity::TYPE("BuilderEntity");

    BuilderEntity::BuilderEntity(const DataUnit* data)
    : id(0)
    , level(0)
    , side(Side::ally)
    , position()
    , data(nullptr)
    , user(nullptr)
    , _reference_counter(1)
    {
        this->data = data;
    }

    BuilderEntity::~BuilderEntity()
    {
    }

    BuilderEntity& BuilderEntity::set_level(int level)
    {
        this->level = level;
        return *this;
    }

    BuilderEntity& BuilderEntity::set_side(Side side)
    {
        this->side = side;
        return *this;
    }

    BuilderEntity& BuilderEntity::set_user(ModelUser* user)
    {
        this->user = user;
        return *this;
    }

    BuilderEntity& BuilderEntity::set_position(const Vector& pos)
    {
        this->position = pos;
        return *this;
    }

    int BuilderEntity::build(ModelEcsBase* model)
    {
        assert(this->data);
        id = model->get_free_id();
        model->add(make_intrusive<ComponentData>(this->data), id);
        auto stats = make_intrusive<ComponentStats>();
        stats->data = this->data;
        model->add(stats, id);
        if(this->user)
        {
            auto component = make_intrusive<ComponentUser>();
            component->user = this->user;
            model->add(component, id);
        }
        auto transform = make_intrusive<Transform>();
        transform->position = this->position;
        model->add(transform, id);
        auto component_side = make_intrusive<ComponentSide>();
        component_side->side = this->side;
        model->add(component_side, id);
        model->add(make_intrusive<ComponentBusy>(), id);
        if(in_map(UnitStat::size, this->data->stats) && this->data->bullet_type == BulletType::none)
        {
            auto component = make_intrusive<ComponentBody>();
            component->size = stats->get(model, UnitStat::size);
            model->add(component, id);
        }
        if(in_map(UnitStat::recharge, this->data->stats))
        {
            model->add(make_intrusive<ComponentRecharge>(), id);
        }
        if(in_map(UnitStat::health, this->data->stats))
        {
            model->add(make_intrusive<ComponentTargetable>(), id);

            auto health = make_intrusive<ComponentHealth>();
            model->add(health, id);
            health->max = stats->get(model, UnitStat::health);
            health->set_current(health->max);

            model->add(make_intrusive<Freezing>(), id);
        }
        this->add_components_from_data(model, id);
        this->add_components_from_equipment(model, id);
        return id;
    }

    void BuilderEntity::add_components_from_data(ModelEcsBase* model, int id)
    {
        for(auto component : this->data->components)
        {
            auto clone = clone_object<ComponentBase>(component);
            clone->id = id;
            clone->add_self_to_model(model);
        }
    }

    void BuilderEntity::add_components_from_equipment(ModelEcsBase* model, int id)
    {
        if(!this->user)
        {
            return;
        }
        auto equipments = user->system<SystemEquipment>();
        for (auto&& pair : equipments->slots)
        {
            auto& slot = pair.first;
            auto& model_slot = pair.second;
            (void)slot; //don't generate 'Unused variable' warning
            (void)model_slot; //don't generate 'Unused variable' warning
            if(model_slot->item)
            {
                auto components = model_slot->get_ecs_components();
                for(auto component : components)
                {
                    component->id = id;
                    component->add_self_to_model(model);
                }
            }
        }
    }

    void BuilderEntity::retain()
    {
        ++this->_reference_counter;
    }

    int BuilderEntity::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool BuilderEntity::operator ==(const BuilderEntity& rhs) const
    {
        bool result = true;
        result = result && this->id == rhs.id;
        result = result && this->level == rhs.level;
        result = result && this->side == rhs.side;
        result = result && this->position == rhs.position;
        result = result && ((this->user == rhs.user) || (this->user != nullptr && rhs.user != nullptr && *this->user == *rhs.user));
        return result;
    }

    bool BuilderEntity::operator !=(const BuilderEntity& rhs) const
    {
        return !(*this == rhs);
    }

    BuilderEntity::BuilderEntity(const BuilderEntity& rhs)
    {
        this->operator=(rhs);
    }

    const BuilderEntity& BuilderEntity::operator =(const BuilderEntity& rhs)
    {
        this->id = rhs.id;
        this->level = rhs.level;
        this->side = rhs.side;
        this->position = rhs.position;
        this->data = rhs.data;
        this->user = rhs.user;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string BuilderEntity::get_type() const
    {
        return BuilderEntity::TYPE;
    }

    void BuilderEntity::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(id, "id", int(0));
        serializer.serialize(level, "level", int(0));
        serializer.serialize(side, "side");
        serializer.serialize(position, "position");
        serializer.serialize(data, "data");
        serializer.serialize(user, "user");
    }

    void BuilderEntity::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(id, "id", int(0));
        deserializer.deserialize(level, "level", int(0));
        deserializer.deserialize(side, "side");
        deserializer.deserialize(position, "position");
        deserializer.deserialize(data, "data");
        deserializer.deserialize(user, "user");
    }

    void BuilderEntity::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(id, "id", int(0));
        serializer.serialize(level, "level", int(0));
        serializer.serialize(side, "side");
        serializer.serialize(position, "position");
        serializer.serialize(data, "data");
        serializer.serialize(user, "user");
    }

    void BuilderEntity::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(id, "id", int(0));
        deserializer.deserialize(level, "level", int(0));
        deserializer.deserialize(side, "side");
        deserializer.deserialize(position, "position");
        deserializer.deserialize(data, "data");
        deserializer.deserialize(user, "user");
    }

} //namespace mg
