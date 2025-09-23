#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BuilderEntity.h"
#include "ComponentSpawn.h"
#include "ModelEcsBase.h"
#include "Side.h"
#include "SystemSpawn.h"
#include "UnitStat.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemSpawn::TYPE("SystemSpawn");

    SystemSpawn::SystemSpawn()
    : _reference_counter(1)
    {

    }

    SystemSpawn::~SystemSpawn()
    {
    }

    void SystemSpawn::update(ModelEcsBase* model, float dt)
    {
        for(auto& spawn : model->components_spawn)
        {
            spawn->timer -= dt;
            if(spawn->create_marker)
            {
                model->event_create_marker.notify(spawn->position, spawn->timer, spawn->unit->stats.at(UnitStat::size), "spawn_point");
                spawn->create_marker = false;
            }
            if(spawn->timer <= 0)
            {
                this->spawn_unit(model, spawn);
                spawn->clean = true;
            }
        }
        auto iter = std::remove_if(model->components_spawn.begin(), model->components_spawn.end(), [&](const auto& spawn)
        {
            return spawn->clean;
        });
        model->components_spawn.erase(iter, model->components_spawn.end());;
    }

    void SystemSpawn::spawn_unit(ModelEcsBase* model, ComponentSpawn* spawn)
    {
        auto id = BuilderEntity(spawn->unit).set_level(1).set_side(Side::enemy).set_position(spawn->position).build(model);
        model->event_create_entity.notify(id, spawn->position);
    }

    void SystemSpawn::retain()
    {
        ++this->_reference_counter;
    }

    int SystemSpawn::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool SystemSpawn::operator ==(const SystemSpawn& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemSpawn::operator !=(const SystemSpawn& rhs) const
    {
        return !(*this == rhs);
    }

    SystemSpawn::SystemSpawn(const SystemSpawn& rhs)
    {
        this->operator=(rhs);
    }

    const SystemSpawn& SystemSpawn::operator =(const SystemSpawn& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemSpawn::get_type() const
    {
        return SystemSpawn::TYPE;
    }

    void SystemSpawn::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemSpawn::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemSpawn::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemSpawn::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
