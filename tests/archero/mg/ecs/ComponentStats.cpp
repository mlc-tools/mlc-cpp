#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../model/Model.h"
#include "../model/System.h"
#include "../systems/SystemEquipment.h"
#include "ComponentBase.h"
#include "ComponentStats.h"
#include "ComponentUser.h"
#include "DataStatUpgrade.h"
#include "DataUnit.h"
#include "ModelEcsBase.h"
#include "Modifier.h"
#include "UnitStat.h"
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ComponentStats::TYPE("ComponentStats");

    ComponentStats::ComponentStats()
    : parent_id(0)
    , data(nullptr)
    , upgrades()
    , stats()
    , cashe()
    , cashe_string()
    {

    }

    ComponentStats::~ComponentStats()
    {
    }

    void ComponentStats::add_upgrade(const DataStatUpgrade* data)
    {
        assert(data->stats.size() == data->modifiers.size());

        this->upgrades.push_back(data);

        auto size = data->stats.size();
        for(int i=0; i<size; ++i)
        {
            auto& modifiers = this->stats[data->stats[i]];
            modifiers.push_back(data->modifiers[i]);
            modifiers.back().name = data->name;
            std::sort(modifiers.begin(), modifiers.end(), [](const Modifier& lhs, const Modifier& rhs)
            {
                return lhs.priority < rhs.priority;
            });
            map_remove(this->cashe, data->stats[i]);
            map_remove(this->cashe_string, data->stats[i]);
        }
    }

    void ComponentStats::set_fixed(const UnitStat& stat, float value)
    {
        this->cashe[stat] = value;
    }

    void ComponentStats::reset_cashe(const UnitStat& stat)
    {
        map_remove(this->cashe, stat);
    }

    void ComponentStats::remove_upgrade(const DataStatUpgrade* data)
    {
        list_remove(this->upgrades, data);
        for(auto stat : data->stats)
        {
            auto iter = std::remove_if(this->stats[stat].begin(), this->stats[stat].end(), [&](const auto& modifier)
            {
                return modifier.name == data->name;
            });
            this->stats[stat].erase(iter, this->stats[stat].end());;
            map_remove(this->cashe, stat);
            map_remove(this->cashe_string, stat);
        }
    }

    float ComponentStats::get_stat_base(const UnitStat& stat)
    {
        float value = 0;
        if(in_map(stat, data->stats))
        {
            value = data->stats.at(stat);
        }
        return value;
    }

    std::string ComponentStats::get_stat_string_base(const UnitStat& stat)
    {
        std::string value;
        if(in_map(stat, data->stats_string))
        {
            value = data->stats_string.at(stat);
        }
        return value;
    }

    float ComponentStats::get(ModelEcsBase* model, const UnitStat& stat)
    {
        if(in_map(stat, this->cashe))
        {
            return this->cashe[stat];
        }

        auto base = this->get_stat_base(stat);
        float result = 0;
        if(this->parent_id > 0)
        {
            auto parent_stats = model->get<ComponentStats>(this->parent_id);
            if(parent_stats)
            {
                result = parent_stats->get(model, stat);
                result = base * result;
            }
        }
        else
        {
            result = base;
        }

        auto component_user = model->get<ComponentUser>(this->id);
        if(component_user)
        {
            auto system = component_user->user->system<SystemEquipment>();
            if(system)
            {
                result = system->get_stat(stat, result);
            }
        }

        auto& modifiers = this->stats[stat];
        for(auto& modifier : modifiers)
        {
            result = modifier.modify(result);
        }

        this->cashe[stat] = result;
        return result;
    }

    std::string ComponentStats::get_string(ModelEcsBase* model, const UnitStat& stat)
    {
        if(in_map(stat, this->cashe_string))
        {
            return this->cashe_string[stat];
        }

        auto value = this->get_stat_string_base(stat);

        auto component_user = model->get<ComponentUser>(this->id);
        if(component_user)
        {
            auto system = component_user->user->system<SystemEquipment>();
            if(system)
            {
                auto new_value = system->get_stat_string(stat);
                if(!new_value.empty())
                {
                    value = new_value;
                }
            }
        }

        auto& modifiers = this->stats[stat];
        if(list_size(modifiers) > 0)
        {
            auto size = static_cast<int>(modifiers.size());
            for(int i=size-1; i>=0; --i)
            {
                if(!modifiers[i].string_value.empty())
                {
                    value = modifiers[i].string_value;
                    break;
                }
            }
        }
        this->cashe_string[stat] = value;
        return value;
    }

    bool ComponentStats::operator ==(const ComponentStats& rhs) const
    {
        bool result = this->ComponentBase::operator ==(rhs);
        result = result && this->parent_id == rhs.parent_id;
        result = result && ((this->data == rhs.data) || (this->data != nullptr && rhs.data != nullptr && *this->data == *rhs.data));
        result = result && this->upgrades == rhs.upgrades;
        result = result && this->stats == rhs.stats;
        result = result && this->cashe == rhs.cashe;
        result = result && this->cashe_string == rhs.cashe_string;
        return result;
    }

    bool ComponentStats::operator !=(const ComponentStats& rhs) const
    {
        return !(*this == rhs);
    }

    ComponentStats::ComponentStats(const ComponentStats& rhs)
    {
        this->operator=(rhs);
    }

    const ComponentStats& ComponentStats::operator =(const ComponentStats& rhs)
    {
        this->ComponentBase::operator=(rhs);
        this->parent_id = rhs.parent_id;
        this->data = rhs.data;
        this->upgrades = rhs.upgrades;
        this->stats = rhs.stats;
        this->cashe = rhs.cashe;
        this->cashe_string = rhs.cashe_string;
        return *this;
    }

    std::string ComponentStats::get_type() const
    {
        return ComponentStats::TYPE;
    }

    void ComponentStats::add_self_to_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->add<ComponentStats>(this);
    }

    void ComponentStats::remove_self_from_model(ModelEcsBase* model_dungeon_class)
    {
        model_dungeon_class->remove<ComponentStats>(this);
    }

    intrusive_ptr<ComponentBase> ComponentStats::get_self_from_model(ModelEcsBase* model_dungeon_class, int id)
    {
        return model_dungeon_class->get<ComponentStats>(id);
    }

    void ComponentStats::serialize_xml(SerializerXml& serializer) const
    {
        ComponentBase::serialize_xml(serializer);
        serializer.serialize(parent_id, "parent_id", int(0));
        serializer.serialize(data, "data");
        serializer.serialize(upgrades, "upgrades");
        serializer.serialize(stats, "stats");
        serializer.serialize(cashe, "cashe");
        serializer.serialize(cashe_string, "cashe_string");
    }

    void ComponentStats::deserialize_xml(DeserializerXml& deserializer)
    {
        ComponentBase::deserialize_xml(deserializer);
        deserializer.deserialize(parent_id, "parent_id", int(0));
        deserializer.deserialize(data, "data");
        deserializer.deserialize(upgrades, "upgrades");
        deserializer.deserialize(stats, "stats");
        deserializer.deserialize(cashe, "cashe");
        deserializer.deserialize(cashe_string, "cashe_string");
    }

    void ComponentStats::serialize_json(SerializerJson& serializer) const
    {
        ComponentBase::serialize_json(serializer);
        serializer.serialize(parent_id, "parent_id", int(0));
        serializer.serialize(data, "data");
        serializer.serialize(upgrades, "upgrades");
        serializer.serialize(stats, "stats");
        serializer.serialize(cashe, "cashe");
        serializer.serialize(cashe_string, "cashe_string");
    }

    void ComponentStats::deserialize_json(DeserializerJson& deserializer)
    {
        ComponentBase::deserialize_json(deserializer);
        deserializer.deserialize(parent_id, "parent_id", int(0));
        deserializer.deserialize(data, "data");
        deserializer.deserialize(upgrades, "upgrades");
        deserializer.deserialize(stats, "stats");
        deserializer.deserialize(cashe, "cashe");
        deserializer.deserialize(cashe_string, "cashe_string");
    }

} //namespace mg
