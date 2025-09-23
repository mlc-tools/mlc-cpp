#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BuilderEntity.h"
#include "ComponentSpawnSpirit.h"
#include "ComponentStats.h"
#include "ModelEcsBase.h"
#include "Side.h"
#include "SystemSpiritSpawn.h"
#include "Transform.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemSpiritSpawn::TYPE("SystemSpiritSpawn");

    SystemSpiritSpawn::SystemSpiritSpawn()
    : _reference_counter(1)
    {

    }

    SystemSpiritSpawn::~SystemSpiritSpawn()
    {
    }

    void SystemSpiritSpawn::update(ModelEcsBase* model, float dt)
    {
        while(!model->components_spawn_spirit.empty())
        {
            auto spawn = model->components_spawn_spirit.back();
            this->create_spirit(model, spawn);
            model->remove(spawn);
        }
    }

    void SystemSpiritSpawn::create_spirit(ModelEcsBase* model, ComponentSpawnSpirit* spawn)
    {
        auto transform = model->get<Transform>(model->player_id);
        auto id = BuilderEntity(spawn->spirit).set_position(transform->position).set_side(Side::ally).build(model);
        auto stats = model->get<ComponentStats>(id);
        stats->parent_id = model->player_id;
        model->event_create_entity.notify(id, transform->position);
    }

    void SystemSpiritSpawn::retain()
    {
        ++this->_reference_counter;
    }

    int SystemSpiritSpawn::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemSpiritSpawn::operator ==(const SystemSpiritSpawn& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemSpiritSpawn::operator !=(const SystemSpiritSpawn& rhs) const
    {
        return !(*this == rhs);
    }

    SystemSpiritSpawn::SystemSpiritSpawn(const SystemSpiritSpawn& rhs)
    {
        this->operator=(rhs);
    }

    const SystemSpiritSpawn& SystemSpiritSpawn::operator =(const SystemSpiritSpawn& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemSpiritSpawn::get_type() const
    {
        return SystemSpiritSpawn::TYPE;
    }

    void SystemSpiritSpawn::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemSpiritSpawn::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemSpiritSpawn::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemSpiritSpawn::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
