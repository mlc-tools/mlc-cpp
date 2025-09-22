#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "../data/DataParams.h"
#include "../utils/Shuffle.h"
#include "BuilderEntity.h"
#include "ComponentBase.h"
#include "ComponentSide.h"
#include "ComponentSpawn.h"
#include "ComponentSpirit.h"
#include "ComponentStats.h"
#include "ControllerDungeonBase.h"
#include "DataStatUpgrade.h"
#include "DataUnit.h"
#include "Side.h"
#include "SpawnInfo.h"
#include "SystemMovement.h"
#include "SystemResolveCollisions.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ControllerDungeonBase::TYPE("ControllerDungeonBase");

    ControllerDungeonBase::ControllerDungeonBase()
    : model(nullptr)
    , paused(false)
    , wave_counter(0)
    , show_window_skill(false)
    , run_next_wave(false)
    {

    }

    ControllerDungeonBase::~ControllerDungeonBase()
    {
    }

    void ControllerDungeonBase::pause()
    {
        paused += 1;
    }

    void ControllerDungeonBase::resume()
    {
        paused -= 1;
    }

    bool ControllerDungeonBase::is_level_finished()
    {
        return this->model->wave_index >= this->model->data->waves.size() && this->is_wave_finished();
    }

    bool ControllerDungeonBase::is_wave_finished()
    {
        if(model->components_spawn.size() > 0)
        {
            return false;
        }
        for(auto component : model->components_side)
        {
            if(component->side == Side::enemy)
            {
                return false;
            }
        }
        ;
        if(model->wave_index > 0 && model->wave_index-1 < list_size(model->data->waves))
        {
            auto wave_generator = model->data->waves.at(model->wave_index-1);
            if(wave_generator->is_interval_generate())
            {
                return wave_generator->is_finished(model);
            }
        }
        model->wave_finished = true;
        return true;
    }

    void ControllerDungeonBase::update(float dt)
    {
        if(is_level_finished())
        {
            return;
        }
        if(this->paused == 0 && dt > 0)
        {
            model->update(dt);
        }
        if(this->paused == 0 && is_wave_finished() == true && !this->show_window_skill)
        {
            if(!model->data->is_change_view_on_next_wave())
            {
                generate_wave();
            }
            else if(model->data->is_change_view_on_next_wave() && model->components_gate.size() == false && !run_next_wave && model->spawn_points.count("gate") > 0)
            {
                this->create_gate_to_next_wave();
            }
            else
            {
                generate_wave();
            }
        }
        auto wave_generator = model->data->waves.at(model->wave_index-1);
        if(wave_generator->is_interval_generate() && !this->show_window_skill)
        {
            if(!wave_generator->is_finished(model) && wave_generator->update(model, dt))
            {
                this->generate_units_on_wave();
            }
        }
    }

    void ControllerDungeonBase::create_hero()
    {
        auto data = DataStorage::shared().get<DataUnit>("hero_survival");
        auto pos = model->spawn_points.at("hero");
        model->player_id = BuilderEntity(data).set_position(pos).set_side(Side::ally).set_user(model->user).build(model);
        model->event_create_entity.notify(model->player_id, pos);
    }

    void ControllerDungeonBase::create_gate_to_next_wave()
    {
        auto data = DataStorage::shared().get<DataUnit>("entity_gate_to_next_wave");
        auto pos = model->spawn_points.at("gate");
        auto id = BuilderEntity(data).set_position(pos).set_side(Side::ally).build(model);
        model->event_create_entity.notify(id, pos);
    }

    void ControllerDungeonBase::generate_wave()
    {
        map_clear(SystemResolveCollisions::event_collision);
        map_clear(SystemMovement::event_on_wall);
        ++model->wave_index;
        this->model->data->waves.at(model->wave_index-1)->on_start(this->model);
        this->generate_units_on_wave();
        model->wave_finished = false;
        model->event_wave_start.notify();
    }

    void ControllerDungeonBase::generate_units_on_wave()
    {
        assert(model->wave_index > 0 && model->wave_index-1 < list_size(model->data->waves));
        auto wave_spawn = model->data->waves.at(model->wave_index-1);
        auto spawns = wave_spawn->get_units(model);
        for(auto& spawn : spawns)
        {
            this->create_enemy(spawn.unit, spawn.position, 1);
        }
    }

    void ControllerDungeonBase::create_enemy(const DataUnit* unit, const Vector& pos, int level)
    {
        auto spawn = make_intrusive<ComponentSpawn>();
        spawn->unit = unit;
        spawn->position = pos;
        model->add(spawn, model->get_free_id());
    }

    void ControllerDungeonBase::add_skill(const DataStatUpgrade* skill)
    {
        if(!skill->to_spirits)
        {
            add_skill_to_entity(skill, this->model->player_id);
        }
        else
        {
            this->model->each<ComponentSpirit>([&](auto& spirit)
            {
                add_skill_to_entity(skill, spirit->id);
            });
        }
        this->model->has_skills[skill] = 1;
    }

    void ControllerDungeonBase::add_skill_to_entity(const DataStatUpgrade* skill, int entity_id)
    {
        auto stats = this->model->get<ComponentStats>(entity_id);
        stats->add_upgrade(skill);
        for(auto& component : skill->components)
        {
            auto clone = clone_object<ComponentBase>(component);
            clone->id = entity_id;
            clone->add_self_to_model(model);
        }
    }

    std::vector<const DataStatUpgrade*> ControllerDungeonBase::generate_skills(int count)
    {
        auto params = DataStorage::shared().get<DataParams>(DataParams::DEFAULT);
        auto rarity = Shuffle::shuffle(params->skill_rarity_weights);
        std::vector<const DataStatUpgrade*> skills;
        std::vector<const DataStatUpgrade*> result;
        for (auto&& pair : DataStorage::shared().get_stat_upgrades())
        {
            auto& name = pair.first;
            auto& data = pair.second;
            (void)name; //don't generate 'Unused variable' warning
            (void)data; //don't generate 'Unused variable' warning
            if(data.rarity == rarity && !in_map(&data, this->model->has_skills))
            {
                skills.push_back(&data);
            }
        }
        while(count-- > 0)
        {
            auto i = random_int(0, static_cast<int>(skills.size()));
            result.push_back(skills[i]);
            list_erase(skills, i);
        }
        return result;
    }

    bool ControllerDungeonBase::operator ==(const ControllerDungeonBase& rhs) const
    {
        bool result = this->IControllerDungeonBase::operator ==(rhs);
        result = result && ((this->model == rhs.model) || (this->model != nullptr && rhs.model != nullptr && *this->model == *rhs.model));
        result = result && this->paused == rhs.paused;
        result = result && this->wave_counter == rhs.wave_counter;
        result = result && this->show_window_skill == rhs.show_window_skill;
        result = result && this->run_next_wave == rhs.run_next_wave;
        return result;
    }

    bool ControllerDungeonBase::operator !=(const ControllerDungeonBase& rhs) const
    {
        return !(*this == rhs);
    }

    ControllerDungeonBase::ControllerDungeonBase(const ControllerDungeonBase& rhs)
    {
        this->operator=(rhs);
    }

    const ControllerDungeonBase& ControllerDungeonBase::operator =(const ControllerDungeonBase& rhs)
    {
        this->IControllerDungeonBase::operator=(rhs);
        this->model = rhs.model;
        this->paused = rhs.paused;
        this->wave_counter = rhs.wave_counter;
        this->show_window_skill = rhs.show_window_skill;
        this->run_next_wave = rhs.run_next_wave;
        return *this;
    }

    std::string ControllerDungeonBase::get_type() const
    {
        return ControllerDungeonBase::TYPE;
    }

    void ControllerDungeonBase::serialize_xml(SerializerXml& serializer) const
    {
        IControllerDungeonBase::serialize_xml(serializer);
        serializer.serialize(model, "model");
        serializer.serialize(paused, "paused", int(false));
        serializer.serialize(wave_counter, "wave_counter", int(0));
        serializer.serialize(show_window_skill, "show_window_skill", bool(false));
        serializer.serialize(run_next_wave, "run_next_wave", bool(false));
    }

    void ControllerDungeonBase::deserialize_xml(DeserializerXml& deserializer)
    {
        IControllerDungeonBase::deserialize_xml(deserializer);
        deserializer.deserialize(model, "model");
        deserializer.deserialize(paused, "paused", int(false));
        deserializer.deserialize(wave_counter, "wave_counter", int(0));
        deserializer.deserialize(show_window_skill, "show_window_skill", bool(false));
        deserializer.deserialize(run_next_wave, "run_next_wave", bool(false));
    }

    void ControllerDungeonBase::serialize_json(SerializerJson& serializer) const
    {
        IControllerDungeonBase::serialize_json(serializer);
        serializer.serialize(model, "model");
        serializer.serialize(paused, "paused", int(false));
        serializer.serialize(wave_counter, "wave_counter", int(0));
        serializer.serialize(show_window_skill, "show_window_skill", bool(false));
        serializer.serialize(run_next_wave, "run_next_wave", bool(false));
    }

    void ControllerDungeonBase::deserialize_json(DeserializerJson& deserializer)
    {
        IControllerDungeonBase::deserialize_json(deserializer);
        deserializer.deserialize(model, "model");
        deserializer.deserialize(paused, "paused", int(false));
        deserializer.deserialize(wave_counter, "wave_counter", int(0));
        deserializer.deserialize(show_window_skill, "show_window_skill", bool(false));
        deserializer.deserialize(run_next_wave, "run_next_wave", bool(false));
    }

} //namespace mg
