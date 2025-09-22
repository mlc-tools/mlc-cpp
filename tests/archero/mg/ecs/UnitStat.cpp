#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../BaseEnum.h"
#include "UnitStat.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{

    UnitStat::UnitStat()
    {

    }


    UnitStat::UnitStat(int _value)
    {
        value = _value;
    }

    UnitStat::UnitStat(const UnitStat& rhs)
    {
        value = rhs.value;
    }

    UnitStat::UnitStat(const std::string& _value)
    {
        if(_value == "none")
        {
            value = none;
            return;
        }
        if(_value == "health")
        {
            value = health;
            return;
        }
        if(_value == "damage")
        {
            value = damage;
            return;
        }
        if(_value == "speed")
        {
            value = speed;
            return;
        }
        if(_value == "size")
        {
            value = size;
            return;
        }
        if(_value == "recharge")
        {
            value = recharge;
            return;
        }
        if(_value == "bullet")
        {
            value = bullet;
            return;
        }
        if(_value == "shoot_radius")
        {
            value = shoot_radius;
            return;
        }
        if(_value == "mass_damage")
        {
            value = mass_damage;
            return;
        }
        if(_value == "damage_mass_radius")
        {
            value = damage_mass_radius;
            return;
        }
        if(_value == "attack_on_move")
        {
            value = attack_on_move;
            return;
        }
        if(_value == "push_target")
        {
            value = push_target;
            return;
        }
        if(_value == "push_target_resist")
        {
            value = push_target_resist;
            return;
        }
        if(_value == "bullets_count")
        {
            value = bullets_count;
            return;
        }
        if(_value == "bullets_count_side")
        {
            value = bullets_count_side;
            return;
        }
        if(_value == "bullets_count_side_deviation")
        {
            value = bullets_count_side_deviation;
            return;
        }
        if(_value == "bullets_count_back")
        {
            value = bullets_count_back;
            return;
        }
        if(_value == "bullet_follow_target")
        {
            value = bullet_follow_target;
            return;
        }
        if(_value == "bullets_through_walls")
        {
            value = bullets_through_walls;
            return;
        }
        if(_value == "bullets_pierce")
        {
            value = bullets_pierce;
            return;
        }
        if(_value == "sword_time_1")
        {
            value = sword_time_1;
            return;
        }
        if(_value == "sword_time_10")
        {
            value = sword_time_10;
            return;
        }
        if(_value == "sword_wave")
        {
            value = sword_wave;
            return;
        }
        if(_value == "sword_count_mul")
        {
            value = sword_count_mul;
            return;
        }
        if(_value == "sword_damage")
        {
            value = sword_damage;
            return;
        }
        if(_value == "multi_shoot")
        {
            value = multi_shoot;
            return;
        }
        if(_value == "dodge")
        {
            value = dodge;
            return;
        }
        if(_value == "ressurection")
        {
            value = ressurection;
            return;
        }
        if(_value == "damage_after_damaged")
        {
            value = damage_after_damaged;
            return;
        }
        if(_value == "damage_after_kill")
        {
            value = damage_after_kill;
            return;
        }
        if(_value == "freeze")
        {
            value = freeze;
            return;
        }
        if(_value == "freeze_immunity_time")
        {
            value = freeze_immunity_time;
            return;
        }
        if(_value == "freezing_resist")
        {
            value = freezing_resist;
            return;
        }
        if(_value == "freeze_chance")
        {
            value = freeze_chance;
            return;
        }
        if(_value == "fire_damage")
        {
            value = fire_damage;
            return;
        }
        if(_value == "fire_resist")
        {
            value = fire_resist;
            return;
        }
        if(_value == "electric_damage")
        {
            value = electric_damage;
            return;
        }
        if(_value == "electric_resist")
        {
            value = electric_resist;
            return;
        }
        if(_value == "stun_chance")
        {
            value = stun_chance;
            return;
        }
        if(_value == "stun_duration")
        {
            value = stun_duration;
            return;
        }
        if(_value == "ricochet_bounce")
        {
            value = ricochet_bounce;
            return;
        }
        if(_value == "ricochet_wall")
        {
            value = ricochet_wall;
            return;
        }
        if(_value == "meteor_chance")
        {
            value = meteor_chance;
            return;
        }
        if(_value == "meteor_rain")
        {
            value = meteor_rain;
            return;
        }
        if(_value == "meteor_count")
        {
            value = meteor_count;
            return;
        }
        if(_value == "damage_distance")
        {
            value = damage_distance;
            return;
        }
        if(_value == "bullet_split")
        {
            value = bullet_split;
            return;
        }
        if(_value == "damage_to_boss")
        {
            value = damage_to_boss;
            return;
        }
        value = 0;
    }

    const UnitStat& UnitStat::operator =(const UnitStat& rhs)
    {
        value = rhs.value;
        return *this;
    }

    const UnitStat& UnitStat::operator =(int rhs)
    {
        value = rhs;
        return *this;
    }

    const UnitStat& UnitStat::operator =(const std::string& _value)
    {
        if(_value == "none")
        {
            value = none;
            return *this;
        }
        if(_value == "health")
        {
            value = health;
            return *this;
        }
        if(_value == "damage")
        {
            value = damage;
            return *this;
        }
        if(_value == "speed")
        {
            value = speed;
            return *this;
        }
        if(_value == "size")
        {
            value = size;
            return *this;
        }
        if(_value == "recharge")
        {
            value = recharge;
            return *this;
        }
        if(_value == "bullet")
        {
            value = bullet;
            return *this;
        }
        if(_value == "shoot_radius")
        {
            value = shoot_radius;
            return *this;
        }
        if(_value == "mass_damage")
        {
            value = mass_damage;
            return *this;
        }
        if(_value == "damage_mass_radius")
        {
            value = damage_mass_radius;
            return *this;
        }
        if(_value == "attack_on_move")
        {
            value = attack_on_move;
            return *this;
        }
        if(_value == "push_target")
        {
            value = push_target;
            return *this;
        }
        if(_value == "push_target_resist")
        {
            value = push_target_resist;
            return *this;
        }
        if(_value == "bullets_count")
        {
            value = bullets_count;
            return *this;
        }
        if(_value == "bullets_count_side")
        {
            value = bullets_count_side;
            return *this;
        }
        if(_value == "bullets_count_side_deviation")
        {
            value = bullets_count_side_deviation;
            return *this;
        }
        if(_value == "bullets_count_back")
        {
            value = bullets_count_back;
            return *this;
        }
        if(_value == "bullet_follow_target")
        {
            value = bullet_follow_target;
            return *this;
        }
        if(_value == "bullets_through_walls")
        {
            value = bullets_through_walls;
            return *this;
        }
        if(_value == "bullets_pierce")
        {
            value = bullets_pierce;
            return *this;
        }
        if(_value == "sword_time_1")
        {
            value = sword_time_1;
            return *this;
        }
        if(_value == "sword_time_10")
        {
            value = sword_time_10;
            return *this;
        }
        if(_value == "sword_wave")
        {
            value = sword_wave;
            return *this;
        }
        if(_value == "sword_count_mul")
        {
            value = sword_count_mul;
            return *this;
        }
        if(_value == "sword_damage")
        {
            value = sword_damage;
            return *this;
        }
        if(_value == "multi_shoot")
        {
            value = multi_shoot;
            return *this;
        }
        if(_value == "dodge")
        {
            value = dodge;
            return *this;
        }
        if(_value == "ressurection")
        {
            value = ressurection;
            return *this;
        }
        if(_value == "damage_after_damaged")
        {
            value = damage_after_damaged;
            return *this;
        }
        if(_value == "damage_after_kill")
        {
            value = damage_after_kill;
            return *this;
        }
        if(_value == "freeze")
        {
            value = freeze;
            return *this;
        }
        if(_value == "freeze_immunity_time")
        {
            value = freeze_immunity_time;
            return *this;
        }
        if(_value == "freezing_resist")
        {
            value = freezing_resist;
            return *this;
        }
        if(_value == "freeze_chance")
        {
            value = freeze_chance;
            return *this;
        }
        if(_value == "fire_damage")
        {
            value = fire_damage;
            return *this;
        }
        if(_value == "fire_resist")
        {
            value = fire_resist;
            return *this;
        }
        if(_value == "electric_damage")
        {
            value = electric_damage;
            return *this;
        }
        if(_value == "electric_resist")
        {
            value = electric_resist;
            return *this;
        }
        if(_value == "stun_chance")
        {
            value = stun_chance;
            return *this;
        }
        if(_value == "stun_duration")
        {
            value = stun_duration;
            return *this;
        }
        if(_value == "ricochet_bounce")
        {
            value = ricochet_bounce;
            return *this;
        }
        if(_value == "ricochet_wall")
        {
            value = ricochet_wall;
            return *this;
        }
        if(_value == "meteor_chance")
        {
            value = meteor_chance;
            return *this;
        }
        if(_value == "meteor_rain")
        {
            value = meteor_rain;
            return *this;
        }
        if(_value == "meteor_count")
        {
            value = meteor_count;
            return *this;
        }
        if(_value == "damage_distance")
        {
            value = damage_distance;
            return *this;
        }
        if(_value == "bullet_split")
        {
            value = bullet_split;
            return *this;
        }
        if(_value == "damage_to_boss")
        {
            value = damage_to_boss;
            return *this;
        }
        return *this;
    }

    bool UnitStat::operator ==(const UnitStat& rhs) const
    {
        return value == rhs.value;
    }

    bool UnitStat::operator ==(const BaseEnum& rhs) const
    {
        return value == rhs.operator int();
    }

    bool UnitStat::operator ==(int rhs) const
    {
        return value == rhs;
    }

    bool UnitStat::operator ==(const std::string& rhs) const
    {
        return *this == UnitStat(rhs);
    }

    bool operator ==(const std::string& lhs, const UnitStat& rhs)
    {
        return UnitStat(lhs) == rhs;
    }

    bool UnitStat::operator <(const UnitStat& rhs) const
    {
        return value < rhs.value;
    }

    UnitStat::operator int() const
    {
        return value;
    }

    UnitStat::operator std::string() const
    {
        if(value == none)
        {
            return "none";
        }
        if(value == health)
        {
            return "health";
        }
        if(value == damage)
        {
            return "damage";
        }
        if(value == speed)
        {
            return "speed";
        }
        if(value == size)
        {
            return "size";
        }
        if(value == recharge)
        {
            return "recharge";
        }
        if(value == bullet)
        {
            return "bullet";
        }
        if(value == shoot_radius)
        {
            return "shoot_radius";
        }
        if(value == mass_damage)
        {
            return "mass_damage";
        }
        if(value == damage_mass_radius)
        {
            return "damage_mass_radius";
        }
        if(value == attack_on_move)
        {
            return "attack_on_move";
        }
        if(value == push_target)
        {
            return "push_target";
        }
        if(value == push_target_resist)
        {
            return "push_target_resist";
        }
        if(value == bullets_count)
        {
            return "bullets_count";
        }
        if(value == bullets_count_side)
        {
            return "bullets_count_side";
        }
        if(value == bullets_count_side_deviation)
        {
            return "bullets_count_side_deviation";
        }
        if(value == bullets_count_back)
        {
            return "bullets_count_back";
        }
        if(value == bullet_follow_target)
        {
            return "bullet_follow_target";
        }
        if(value == bullets_through_walls)
        {
            return "bullets_through_walls";
        }
        if(value == bullets_pierce)
        {
            return "bullets_pierce";
        }
        if(value == sword_time_1)
        {
            return "sword_time_1";
        }
        if(value == sword_time_10)
        {
            return "sword_time_10";
        }
        if(value == sword_wave)
        {
            return "sword_wave";
        }
        if(value == sword_count_mul)
        {
            return "sword_count_mul";
        }
        if(value == sword_damage)
        {
            return "sword_damage";
        }
        if(value == multi_shoot)
        {
            return "multi_shoot";
        }
        if(value == dodge)
        {
            return "dodge";
        }
        if(value == ressurection)
        {
            return "ressurection";
        }
        if(value == damage_after_damaged)
        {
            return "damage_after_damaged";
        }
        if(value == damage_after_kill)
        {
            return "damage_after_kill";
        }
        if(value == freeze)
        {
            return "freeze";
        }
        if(value == freeze_immunity_time)
        {
            return "freeze_immunity_time";
        }
        if(value == freezing_resist)
        {
            return "freezing_resist";
        }
        if(value == freeze_chance)
        {
            return "freeze_chance";
        }
        if(value == fire_damage)
        {
            return "fire_damage";
        }
        if(value == fire_resist)
        {
            return "fire_resist";
        }
        if(value == electric_damage)
        {
            return "electric_damage";
        }
        if(value == electric_resist)
        {
            return "electric_resist";
        }
        if(value == stun_chance)
        {
            return "stun_chance";
        }
        if(value == stun_duration)
        {
            return "stun_duration";
        }
        if(value == ricochet_bounce)
        {
            return "ricochet_bounce";
        }
        if(value == ricochet_wall)
        {
            return "ricochet_wall";
        }
        if(value == meteor_chance)
        {
            return "meteor_chance";
        }
        if(value == meteor_rain)
        {
            return "meteor_rain";
        }
        if(value == meteor_count)
        {
            return "meteor_count";
        }
        if(value == damage_distance)
        {
            return "damage_distance";
        }
        if(value == bullet_split)
        {
            return "bullet_split";
        }
        if(value == damage_to_boss)
        {
            return "damage_to_boss";
        }
        return std::string();
    }

    std::string UnitStat::str() const
    {
        if(value == none)
        {
            return "none";
        }
        if(value == health)
        {
            return "health";
        }
        if(value == damage)
        {
            return "damage";
        }
        if(value == speed)
        {
            return "speed";
        }
        if(value == size)
        {
            return "size";
        }
        if(value == recharge)
        {
            return "recharge";
        }
        if(value == bullet)
        {
            return "bullet";
        }
        if(value == shoot_radius)
        {
            return "shoot_radius";
        }
        if(value == mass_damage)
        {
            return "mass_damage";
        }
        if(value == damage_mass_radius)
        {
            return "damage_mass_radius";
        }
        if(value == attack_on_move)
        {
            return "attack_on_move";
        }
        if(value == push_target)
        {
            return "push_target";
        }
        if(value == push_target_resist)
        {
            return "push_target_resist";
        }
        if(value == bullets_count)
        {
            return "bullets_count";
        }
        if(value == bullets_count_side)
        {
            return "bullets_count_side";
        }
        if(value == bullets_count_side_deviation)
        {
            return "bullets_count_side_deviation";
        }
        if(value == bullets_count_back)
        {
            return "bullets_count_back";
        }
        if(value == bullet_follow_target)
        {
            return "bullet_follow_target";
        }
        if(value == bullets_through_walls)
        {
            return "bullets_through_walls";
        }
        if(value == bullets_pierce)
        {
            return "bullets_pierce";
        }
        if(value == sword_time_1)
        {
            return "sword_time_1";
        }
        if(value == sword_time_10)
        {
            return "sword_time_10";
        }
        if(value == sword_wave)
        {
            return "sword_wave";
        }
        if(value == sword_count_mul)
        {
            return "sword_count_mul";
        }
        if(value == sword_damage)
        {
            return "sword_damage";
        }
        if(value == multi_shoot)
        {
            return "multi_shoot";
        }
        if(value == dodge)
        {
            return "dodge";
        }
        if(value == ressurection)
        {
            return "ressurection";
        }
        if(value == damage_after_damaged)
        {
            return "damage_after_damaged";
        }
        if(value == damage_after_kill)
        {
            return "damage_after_kill";
        }
        if(value == freeze)
        {
            return "freeze";
        }
        if(value == freeze_immunity_time)
        {
            return "freeze_immunity_time";
        }
        if(value == freezing_resist)
        {
            return "freezing_resist";
        }
        if(value == freeze_chance)
        {
            return "freeze_chance";
        }
        if(value == fire_damage)
        {
            return "fire_damage";
        }
        if(value == fire_resist)
        {
            return "fire_resist";
        }
        if(value == electric_damage)
        {
            return "electric_damage";
        }
        if(value == electric_resist)
        {
            return "electric_resist";
        }
        if(value == stun_chance)
        {
            return "stun_chance";
        }
        if(value == stun_duration)
        {
            return "stun_duration";
        }
        if(value == ricochet_bounce)
        {
            return "ricochet_bounce";
        }
        if(value == ricochet_wall)
        {
            return "ricochet_wall";
        }
        if(value == meteor_chance)
        {
            return "meteor_chance";
        }
        if(value == meteor_rain)
        {
            return "meteor_rain";
        }
        if(value == meteor_count)
        {
            return "meteor_count";
        }
        if(value == damage_distance)
        {
            return "damage_distance";
        }
        if(value == bullet_split)
        {
            return "bullet_split";
        }
        if(value == damage_to_boss)
        {
            return "damage_to_boss";
        }
        return std::string();
    }

} //namespace mg
