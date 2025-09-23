#ifndef __mg_UnitStat_h__
#define __mg_UnitStat_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../BaseEnum.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class BaseEnum;

    class UnitStat : public BaseEnum
    {
    public:
        UnitStat();

        UnitStat(const BaseEnum& rhs):BaseEnum(rhs){}
        const UnitStat& operator =(const BaseEnum& rhs) { this->value = rhs.operator int(); return *this; }
        UnitStat(int _value);
        UnitStat(const UnitStat& rhs);
        UnitStat(const std::string& _value);
        const UnitStat& operator =(const UnitStat& rhs);
        const UnitStat& operator =(int rhs);
        const UnitStat& operator =(const std::string& _value);
        bool operator ==(const UnitStat& rhs) const;
        bool operator ==(const BaseEnum& rhs) const;
        bool operator ==(int rhs) const;
        bool operator ==(const std::string& rhs) const;
        friend bool operator ==(const std::string& lhs, const UnitStat& rhs);
        bool operator <(const UnitStat& rhs) const;
        operator int() const;
        operator std::string() const;
        std::string str() const;

        static constexpr BaseEnum none = 0;
        static constexpr BaseEnum health = 1;
        static constexpr BaseEnum damage = 2;
        static constexpr BaseEnum speed = 3;
        static constexpr BaseEnum size = 4;
        static constexpr BaseEnum recharge = 5;
        static constexpr BaseEnum bullet = 6;
        static constexpr BaseEnum shoot_radius = 7;
        static constexpr BaseEnum mass_damage = 8;
        static constexpr BaseEnum damage_mass_radius = 9;
        static constexpr BaseEnum attack_on_move = 10;
        static constexpr BaseEnum push_target = 11;
        static constexpr BaseEnum push_target_resist = 12;
        static constexpr BaseEnum bullets_count = 13;
        static constexpr BaseEnum bullets_count_side = 14;
        static constexpr BaseEnum bullets_count_side_deviation = 15;
        static constexpr BaseEnum bullets_count_back = 16;
        static constexpr BaseEnum bullet_follow_target = 17;
        static constexpr BaseEnum bullets_through_walls = 18;
        static constexpr BaseEnum bullets_pierce = 19;
        static constexpr BaseEnum sword_time_1 = 20;
        static constexpr BaseEnum sword_time_10 = 21;
        static constexpr BaseEnum sword_wave = 22;
        static constexpr BaseEnum sword_count_mul = 23;
        static constexpr BaseEnum sword_damage = 24;
        static constexpr BaseEnum multi_shoot = 25;
        static constexpr BaseEnum dodge = 26;
        static constexpr BaseEnum ressurection = 27;
        static constexpr BaseEnum damage_after_damaged = 28;
        static constexpr BaseEnum damage_after_kill = 29;
        static constexpr BaseEnum freeze = 30;
        static constexpr BaseEnum freeze_immunity_time = 31;
        static constexpr BaseEnum freezing_resist = 32;
        static constexpr BaseEnum freeze_chance = 33;
        static constexpr BaseEnum fire_damage = 34;
        static constexpr BaseEnum fire_resist = 35;
        static constexpr BaseEnum electric_damage = 36;
        static constexpr BaseEnum electric_resist = 37;
        static constexpr BaseEnum stun_chance = 38;
        static constexpr BaseEnum stun_duration = 39;
        static constexpr BaseEnum ricochet_bounce = 40;
        static constexpr BaseEnum ricochet_wall = 41;
        static constexpr BaseEnum meteor_chance = 42;
        static constexpr BaseEnum meteor_rain = 43;
        static constexpr BaseEnum meteor_count = 44;
        static constexpr BaseEnum damage_distance = 45;
        static constexpr BaseEnum bullet_split = 46;
        static constexpr BaseEnum damage_to_boss = 47;

    };
} //namespace mg

#endif //#ifndef __mg_UnitStat_h__
