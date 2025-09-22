#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../DataStorage.h"
#include "ActionCreateMarkerNearHero.h"
#include "ComponentStats.h"
#include "DataUnit.h"
#include "Transform.h"
#include "UnitStat.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ActionCreateMarkerNearHero::TYPE("ActionCreateMarkerNearHero");

    ActionCreateMarkerNearHero::ActionCreateMarkerNearHero()
    : marker("")
    , marker_id("marker")
    , radius(0)
    {

    }

    ActionCreateMarkerNearHero::~ActionCreateMarkerNearHero()
    {
    }

    void ActionCreateMarkerNearHero::on_start()
    {
        auto bullet_name = model->get<ComponentStats>(this->entity_id)->get_string(model, UnitStat::bullet);
        auto data = DataStorage::shared().get<DataUnit>(bullet_name);
        auto radius_damage = data->stats.at(UnitStat::damage_mass_radius);
        auto radius_near_player = this->radius;
        if(radius_near_player == 0)
        {
            radius_near_player = radius_damage;
        }
        auto transform_hero = model->get<Transform>(model->player_id);
        auto pos = transform_hero->position + Vector::generate_random_point_in_radius(radius_near_player);
        if(!model->ground->is_bound(pos) || model->ground->has_collision_with_walls(pos, 0))
        {
            pos = transform_hero->position + Vector::generate_random_point_in_radius(radius_near_player);
        }
        common_dictionary->vectors[this->marker_id] = pos;
        if(string_size(this->marker) > 0)
        {
            model->event_create_marker.notify(pos, 2, radius_damage, "");
        }
    }

    bool ActionCreateMarkerNearHero::operator ==(const ActionCreateMarkerNearHero& rhs) const
    {
        bool result = this->ActionBase::operator ==(rhs);
        result = result && this->marker == rhs.marker;
        result = result && this->marker_id == rhs.marker_id;
        result = result && this->radius == rhs.radius;
        return result;
    }

    bool ActionCreateMarkerNearHero::operator !=(const ActionCreateMarkerNearHero& rhs) const
    {
        return !(*this == rhs);
    }

    ActionCreateMarkerNearHero::ActionCreateMarkerNearHero(const ActionCreateMarkerNearHero& rhs)
    {
        this->operator=(rhs);
    }

    const ActionCreateMarkerNearHero& ActionCreateMarkerNearHero::operator =(const ActionCreateMarkerNearHero& rhs)
    {
        this->ActionBase::operator=(rhs);
        this->marker = rhs.marker;
        this->marker_id = rhs.marker_id;
        this->radius = rhs.radius;
        return *this;
    }

    std::string ActionCreateMarkerNearHero::get_type() const
    {
        return ActionCreateMarkerNearHero::TYPE;
    }

    void ActionCreateMarkerNearHero::serialize_xml(SerializerXml& serializer) const
    {
        ActionBase::serialize_xml(serializer);
        serializer.serialize(marker, "marker", std::string(""));
        serializer.serialize(marker_id, "marker_id", std::string("marker"));
        serializer.serialize(radius, "radius", float(0));
    }

    void ActionCreateMarkerNearHero::deserialize_xml(DeserializerXml& deserializer)
    {
        ActionBase::deserialize_xml(deserializer);
        deserializer.deserialize(marker, "marker", std::string(""));
        deserializer.deserialize(marker_id, "marker_id", std::string("marker"));
        deserializer.deserialize(radius, "radius", float(0));
    }

    void ActionCreateMarkerNearHero::serialize_json(SerializerJson& serializer) const
    {
        ActionBase::serialize_json(serializer);
        serializer.serialize(marker, "marker", std::string(""));
        serializer.serialize(marker_id, "marker_id", std::string("marker"));
        serializer.serialize(radius, "radius", float(0));
    }

    void ActionCreateMarkerNearHero::deserialize_json(DeserializerJson& deserializer)
    {
        ActionBase::deserialize_json(deserializer);
        deserializer.deserialize(marker, "marker", std::string(""));
        deserializer.deserialize(marker_id, "marker_id", std::string("marker"));
        deserializer.deserialize(radius, "radius", float(0));
    }

} //namespace mg
