#ifndef __mg_ModelEcsBase_h__
#define __mg_ModelEcsBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../Observable.h"
#include "../model/ModelUser.h"
#include "DataLevel.h"
#include "Ground.h"
#include "intrusive_ptr.h"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ComponentActions;
    class ComponentAura;
    class ComponentAuraDamage;
    class ComponentAuraOfSlow;
    class ComponentBody;
    class ComponentBullet;
    class ComponentBulletFollowToTarget;
    class ComponentBulletLaser;
    class ComponentBulletSplit;
    class ComponentBusy;
    class ComponentCreateMovementToHero;
    class ComponentCreateUnitsOnDeath;
    class ComponentDamage;
    class ComponentData;
    class ComponentDropHeart;
    class ComponentEffects;
    class ComponentExp;
    class ComponentExpDrop;
    class ComponentGate;
    class ComponentHealingLowHp;
    class ComponentHealingOnChangeMaxHp;
    class ComponentHealth;
    class ComponentHeart;
    class ComponentHeartAddStats;
    class ComponentHeartAddStatsDamage;
    class ComponentHeartAddStatsHp;
    class ComponentLevelUp;
    class ComponentMeleeAttack;
    class ComponentMeteorPeriodic;
    class ComponentMovement;
    class ComponentPush;
    class ComponentRandomHealing;
    class ComponentRecharge;
    class ComponentShield;
    class ComponentShootBullet;
    class ComponentSide;
    class ComponentSpawn;
    class ComponentSpawnSpirit;
    class ComponentSphere;
    class ComponentSphereSpawn;
    class ComponentSpineInfo;
    class ComponentSpirit;
    class ComponentSpiritBasePoints;
    class ComponentStats;
    class ComponentStun;
    class ComponentSword;
    class ComponentSwordCast;
    class ComponentTargetHighlight;
    class ComponentTargetable;
    class ComponentUser;
    class ComponentVampire;
    class DataStatUpgrade;
    class ElectricDamage;
    class FireDamage;
    class Freezing;
    class MoveDirection;
    class MoveInstant;
    class MoveParabolic;
    class MoveToTarget;
    class MoveVertical;
    class Task;
    class Transform;
    class Vector;

    class ModelEcsBase
    {
    public:
        ModelEcsBase();
        ~ModelEcsBase();
        template <class T>
        std::vector<intrusive_ptr<T>>& get_components();
        template <class T>
        const std::vector<intrusive_ptr<T>>& get_components() const;
        template <class T>
        intrusive_ptr<T> get(int component_id);
        template <class T>
        const T* get(int component_id) const;
        template <class T>
        void remove(T* component);
        template <class T>
        void remove(intrusive_ptr<T> component);
        int get_free_id();
        void subscribe();
        void unsubscribe();
        void update(float dt);
        void update_systems(float dt);
        void add_entity(int id);
        void remove_entity(int id);
        void on_loaded();
        int get_entity_with_name(const std::string& name) const;
        float get_health_rate() const;
        void retain();
        int release();
        bool operator ==(const ModelEcsBase& rhs) const;
        bool operator !=(const ModelEcsBase& rhs) const;
        ModelEcsBase(const ModelEcsBase& rhs);
        const ModelEcsBase& operator =(const ModelEcsBase& rhs);
        std::string get_type() const;
        void clear_components_actions();
        void clear_components_aura();
        void clear_components_aura_damage();
        void clear_components_aura_of_slow();
        void clear_components_body();
        void clear_components_bullet();
        void clear_components_bullet_follow_to_target();
        void clear_components_bullet_laser();
        void clear_components_bullet_split();
        void clear_components_busy();
        void clear_components_create_movement_to_hero();
        void clear_components_create_units_on_death();
        void clear_components_damage();
        void clear_components_data();
        void clear_components_drop_heart();
        void clear_components_effects();
        void clear_components_exp();
        void clear_components_exp_drop();
        void clear_components_gate();
        void clear_components_healing_low_hp();
        void clear_components_healing_on_change_max_hp();
        void clear_components_health();
        void clear_components_heart();
        void clear_components_heart_add_stats();
        void clear_components_heart_add_stats_damage();
        void clear_components_heart_add_stats_hp();
        void clear_components_level_up();
        void clear_components_melee_attack();
        void clear_components_meteor_periodic();
        void clear_components_movement();
        void clear_components_push();
        void clear_components_random_healing();
        void clear_components_recharge();
        void clear_components_shield();
        void clear_components_shoot_bullet();
        void clear_components_side();
        void clear_components_spawn();
        void clear_components_spawn_spirit();
        void clear_components_sphere();
        void clear_components_sphere_spawn();
        void clear_components_spine_info();
        void clear_components_spirit();
        void clear_components_spirit_base_points();
        void clear_components_stats();
        void clear_components_stun();
        void clear_components_sword();
        void clear_components_sword_cast();
        void clear_components_target_highlight();
        void clear_components_targetable();
        void clear_components_user();
        void clear_components_vampire();
        void clear_components_electric_damage();
        void clear_components_fire_damage();
        void clear_components_freezing();
        void clear_components_move_direction();
        void clear_components_move_instant();
        void clear_components_move_parabolic();
        void clear_components_move_to_target();
        void clear_components_move_vertical();
        void clear_components_transform();
        void build_maps();
        template <class T>
        void add(intrusive_ptr<T> component, int component_id=0);
        template <class T>
        void add(T* component, int component_id=0);
        void copy_entity_from_model(ModelEcsBase* model, int id, int new_id);
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        intrusive_ptr<ModelUser> user;
    private:
        int next_free_id;
    public:
        int player_id;
        std::vector<int> entities;
        float game_timer;
        std::vector<intrusive_ptr<Task>> tasks;
        const DataLevel* data;
        int wave_index;
        intrusive_ptr<Ground> ground;
        int enemies_level;
        int enemies_rank;
        bool wave_finished;
        float timer_wave_interval;
        float timer_wave_duration;
        std::map<const DataStatUpgrade*, int> has_skills;
        Observable<void()> event_did_initialized;
        Observable<void()> event_wave_finish;
        Observable<void()> event_wave_start;
        Observable<void()> event_wave_next;
        Observable<void()> event_exit;
        Observable<void(int, const Vector&)> event_create_entity;
        Observable<void(int, const Vector&)> event_create_bullet;
        Observable<void(int)> event_remove_entity;
        Observable<void(const Vector&, float, float, const std::string&)> event_create_marker;
        Observable<void()> event_hero_level_up;
        Observable<void()> event_hero_exp_changed;
        std::map<int, Observable<void(int)>> event_prepare_to_shoot;
        std::map<int, Observable<void(int)>> event_turn_to;
        std::map<int, Observable<void(int)>> event_shoot;
        std::map<int, Observable<void()>> event_death;
        std::map<int, Observable<void(const std::string&, bool)>> event_skill_animate;
        std::map<int, Observable<void(int)>> event_healing;
        std::map<int, Observable<void()>> event_shield_activated;
        std::map<int, Observable<void()>> event_dodge;
        std::map<int, Observable<void()>> event_ressurection;
        std::map<int, Observable<void(const std::string&)>> event_effects;
        std::map<int, Observable<void()>> event_freeze_activated;
        std::map<int, Observable<void()>> event_freeze_deactivated;
        std::map<int, Observable<void()>> event_fire_activated;
        std::map<int, Observable<void()>> event_fire_deactivated;
        std::map<int, Observable<void()>> event_stun_activated;
        std::map<int, Observable<void()>> event_stun_deactivated;
        std::map<int, Observable<void(float)>> event_damaged;
        std::map<int, Observable<void(ComponentAura*)>> event_aura_activated;
        std::map<std::string, Vector> spawn_points;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;
        std::vector<intrusive_ptr<ComponentActions>> components_actions;
    private:
        std::map<int, intrusive_ptr<ComponentActions>> map_components_actions;
    public:
        std::vector<intrusive_ptr<ComponentAura>> components_aura;
    private:
        std::map<int, intrusive_ptr<ComponentAura>> map_components_aura;
    public:
        std::vector<intrusive_ptr<ComponentAuraDamage>> components_aura_damage;
    private:
        std::map<int, intrusive_ptr<ComponentAuraDamage>> map_components_aura_damage;
    public:
        std::vector<intrusive_ptr<ComponentAuraOfSlow>> components_aura_of_slow;
    private:
        std::map<int, intrusive_ptr<ComponentAuraOfSlow>> map_components_aura_of_slow;
    public:
        std::vector<intrusive_ptr<ComponentBody>> components_body;
    private:
        std::map<int, intrusive_ptr<ComponentBody>> map_components_body;
    public:
        std::vector<intrusive_ptr<ComponentBullet>> components_bullet;
    private:
        std::map<int, intrusive_ptr<ComponentBullet>> map_components_bullet;
    public:
        std::vector<intrusive_ptr<ComponentBulletFollowToTarget>> components_bullet_follow_to_target;
    private:
        std::map<int, intrusive_ptr<ComponentBulletFollowToTarget>> map_components_bullet_follow_to_target;
    public:
        std::vector<intrusive_ptr<ComponentBulletLaser>> components_bullet_laser;
    private:
        std::map<int, intrusive_ptr<ComponentBulletLaser>> map_components_bullet_laser;
    public:
        std::vector<intrusive_ptr<ComponentBulletSplit>> components_bullet_split;
    private:
        std::map<int, intrusive_ptr<ComponentBulletSplit>> map_components_bullet_split;
    public:
        std::vector<intrusive_ptr<ComponentBusy>> components_busy;
    private:
        std::map<int, intrusive_ptr<ComponentBusy>> map_components_busy;
    public:
        std::vector<intrusive_ptr<ComponentCreateMovementToHero>> components_create_movement_to_hero;
    private:
        std::map<int, intrusive_ptr<ComponentCreateMovementToHero>> map_components_create_movement_to_hero;
    public:
        std::vector<intrusive_ptr<ComponentCreateUnitsOnDeath>> components_create_units_on_death;
    private:
        std::map<int, intrusive_ptr<ComponentCreateUnitsOnDeath>> map_components_create_units_on_death;
    public:
        std::vector<intrusive_ptr<ComponentDamage>> components_damage;
    private:
        std::map<int, intrusive_ptr<ComponentDamage>> map_components_damage;
    public:
        std::vector<intrusive_ptr<ComponentData>> components_data;
    private:
        std::map<int, intrusive_ptr<ComponentData>> map_components_data;
    public:
        std::vector<intrusive_ptr<ComponentDropHeart>> components_drop_heart;
    private:
        std::map<int, intrusive_ptr<ComponentDropHeart>> map_components_drop_heart;
    public:
        std::vector<intrusive_ptr<ComponentEffects>> components_effects;
    private:
        std::map<int, intrusive_ptr<ComponentEffects>> map_components_effects;
    public:
        std::vector<intrusive_ptr<ComponentExp>> components_exp;
    private:
        std::map<int, intrusive_ptr<ComponentExp>> map_components_exp;
    public:
        std::vector<intrusive_ptr<ComponentExpDrop>> components_exp_drop;
    private:
        std::map<int, intrusive_ptr<ComponentExpDrop>> map_components_exp_drop;
    public:
        std::vector<intrusive_ptr<ComponentGate>> components_gate;
    private:
        std::map<int, intrusive_ptr<ComponentGate>> map_components_gate;
    public:
        std::vector<intrusive_ptr<ComponentHealingLowHp>> components_healing_low_hp;
    private:
        std::map<int, intrusive_ptr<ComponentHealingLowHp>> map_components_healing_low_hp;
    public:
        std::vector<intrusive_ptr<ComponentHealingOnChangeMaxHp>> components_healing_on_change_max_hp;
    private:
        std::map<int, intrusive_ptr<ComponentHealingOnChangeMaxHp>> map_components_healing_on_change_max_hp;
    public:
        std::vector<intrusive_ptr<ComponentHealth>> components_health;
    private:
        std::map<int, intrusive_ptr<ComponentHealth>> map_components_health;
    public:
        std::vector<intrusive_ptr<ComponentHeart>> components_heart;
    private:
        std::map<int, intrusive_ptr<ComponentHeart>> map_components_heart;
    public:
        std::vector<intrusive_ptr<ComponentHeartAddStats>> components_heart_add_stats;
    private:
        std::map<int, intrusive_ptr<ComponentHeartAddStats>> map_components_heart_add_stats;
    public:
        std::vector<intrusive_ptr<ComponentHeartAddStatsDamage>> components_heart_add_stats_damage;
    private:
        std::map<int, intrusive_ptr<ComponentHeartAddStatsDamage>> map_components_heart_add_stats_damage;
    public:
        std::vector<intrusive_ptr<ComponentHeartAddStatsHp>> components_heart_add_stats_hp;
    private:
        std::map<int, intrusive_ptr<ComponentHeartAddStatsHp>> map_components_heart_add_stats_hp;
    public:
        std::vector<intrusive_ptr<ComponentLevelUp>> components_level_up;
    private:
        std::map<int, intrusive_ptr<ComponentLevelUp>> map_components_level_up;
    public:
        std::vector<intrusive_ptr<ComponentMeleeAttack>> components_melee_attack;
    private:
        std::map<int, intrusive_ptr<ComponentMeleeAttack>> map_components_melee_attack;
    public:
        std::vector<intrusive_ptr<ComponentMeteorPeriodic>> components_meteor_periodic;
    private:
        std::map<int, intrusive_ptr<ComponentMeteorPeriodic>> map_components_meteor_periodic;
    public:
        std::vector<intrusive_ptr<ComponentMovement>> components_movement;
    private:
        std::map<int, intrusive_ptr<ComponentMovement>> map_components_movement;
    public:
        std::vector<intrusive_ptr<ComponentPush>> components_push;
    private:
        std::map<int, intrusive_ptr<ComponentPush>> map_components_push;
    public:
        std::vector<intrusive_ptr<ComponentRandomHealing>> components_random_healing;
    private:
        std::map<int, intrusive_ptr<ComponentRandomHealing>> map_components_random_healing;
    public:
        std::vector<intrusive_ptr<ComponentRecharge>> components_recharge;
    private:
        std::map<int, intrusive_ptr<ComponentRecharge>> map_components_recharge;
    public:
        std::vector<intrusive_ptr<ComponentShield>> components_shield;
    private:
        std::map<int, intrusive_ptr<ComponentShield>> map_components_shield;
    public:
        std::vector<intrusive_ptr<ComponentShootBullet>> components_shoot_bullet;
    private:
        std::map<int, intrusive_ptr<ComponentShootBullet>> map_components_shoot_bullet;
    public:
        std::vector<intrusive_ptr<ComponentSide>> components_side;
    private:
        std::map<int, intrusive_ptr<ComponentSide>> map_components_side;
    public:
        std::vector<intrusive_ptr<ComponentSpawn>> components_spawn;
    private:
        std::map<int, intrusive_ptr<ComponentSpawn>> map_components_spawn;
    public:
        std::vector<intrusive_ptr<ComponentSpawnSpirit>> components_spawn_spirit;
    private:
        std::map<int, intrusive_ptr<ComponentSpawnSpirit>> map_components_spawn_spirit;
    public:
        std::vector<intrusive_ptr<ComponentSphere>> components_sphere;
    private:
        std::map<int, intrusive_ptr<ComponentSphere>> map_components_sphere;
    public:
        std::vector<intrusive_ptr<ComponentSphereSpawn>> components_sphere_spawn;
    private:
        std::map<int, intrusive_ptr<ComponentSphereSpawn>> map_components_sphere_spawn;
    public:
        std::vector<intrusive_ptr<ComponentSpineInfo>> components_spine_info;
    private:
        std::map<int, intrusive_ptr<ComponentSpineInfo>> map_components_spine_info;
    public:
        std::vector<intrusive_ptr<ComponentSpirit>> components_spirit;
    private:
        std::map<int, intrusive_ptr<ComponentSpirit>> map_components_spirit;
    public:
        std::vector<intrusive_ptr<ComponentSpiritBasePoints>> components_spirit_base_points;
    private:
        std::map<int, intrusive_ptr<ComponentSpiritBasePoints>> map_components_spirit_base_points;
    public:
        std::vector<intrusive_ptr<ComponentStats>> components_stats;
    private:
        std::map<int, intrusive_ptr<ComponentStats>> map_components_stats;
    public:
        std::vector<intrusive_ptr<ComponentStun>> components_stun;
    private:
        std::map<int, intrusive_ptr<ComponentStun>> map_components_stun;
    public:
        std::vector<intrusive_ptr<ComponentSword>> components_sword;
    private:
        std::map<int, intrusive_ptr<ComponentSword>> map_components_sword;
    public:
        std::vector<intrusive_ptr<ComponentSwordCast>> components_sword_cast;
    private:
        std::map<int, intrusive_ptr<ComponentSwordCast>> map_components_sword_cast;
    public:
        std::vector<intrusive_ptr<ComponentTargetHighlight>> components_target_highlight;
    private:
        std::map<int, intrusive_ptr<ComponentTargetHighlight>> map_components_target_highlight;
    public:
        std::vector<intrusive_ptr<ComponentTargetable>> components_targetable;
    private:
        std::map<int, intrusive_ptr<ComponentTargetable>> map_components_targetable;
    public:
        std::vector<intrusive_ptr<ComponentUser>> components_user;
    private:
        std::map<int, intrusive_ptr<ComponentUser>> map_components_user;
    public:
        std::vector<intrusive_ptr<ComponentVampire>> components_vampire;
    private:
        std::map<int, intrusive_ptr<ComponentVampire>> map_components_vampire;
    public:
        std::vector<intrusive_ptr<ElectricDamage>> components_electric_damage;
    private:
        std::map<int, intrusive_ptr<ElectricDamage>> map_components_electric_damage;
    public:
        std::vector<intrusive_ptr<FireDamage>> components_fire_damage;
    private:
        std::map<int, intrusive_ptr<FireDamage>> map_components_fire_damage;
    public:
        std::vector<intrusive_ptr<Freezing>> components_freezing;
    private:
        std::map<int, intrusive_ptr<Freezing>> map_components_freezing;
    public:
        std::vector<intrusive_ptr<MoveDirection>> components_move_direction;
    private:
        std::map<int, intrusive_ptr<MoveDirection>> map_components_move_direction;
    public:
        std::vector<intrusive_ptr<MoveInstant>> components_move_instant;
    private:
        std::map<int, intrusive_ptr<MoveInstant>> map_components_move_instant;
    public:
        std::vector<intrusive_ptr<MoveParabolic>> components_move_parabolic;
    private:
        std::map<int, intrusive_ptr<MoveParabolic>> map_components_move_parabolic;
    public:
        std::vector<intrusive_ptr<MoveToTarget>> components_move_to_target;
    private:
        std::map<int, intrusive_ptr<MoveToTarget>> map_components_move_to_target;
    public:
        std::vector<intrusive_ptr<MoveVertical>> components_move_vertical;
    private:
        std::map<int, intrusive_ptr<MoveVertical>> map_components_move_vertical;
    public:
        std::vector<intrusive_ptr<Transform>> components_transform;
    private:
        std::map<int, intrusive_ptr<Transform>> map_components_transform;


    public:
        template <class TCompA, class TCompB>
        std::vector<std::tuple<intrusive_ptr<TCompA>, intrusive_ptr<TCompB>>> view()const
        {
            auto& cont_a = get_components<TCompA>();
            auto& cont_b = get_components<TCompB>();
            assert(std::is_sorted(std::begin(cont_a), std::end(cont_a), [](auto const& lhs, auto const& rhs){return lhs->id < rhs->id;}));
            assert(std::is_sorted(std::begin(cont_b), std::end(cont_b), [](auto const& lhs, auto const& rhs){return lhs->id < rhs->id;}));

            std::vector<std::tuple<intrusive_ptr<TCompA>, intrusive_ptr<TCompB>>> result;
            result.reserve(std::min(cont_a.size(), cont_b.size()));

            size_t i = 0, j = 0;
            auto size_a = cont_a.size();
            auto size_b = cont_b.size();
            while (i < size_a && j < size_b)
            {
                auto& comp_a = cont_a[i];
                auto& comp_b = cont_b[j];
                int ida = comp_a->id;
                int idb = comp_b->id;
                if (ida == idb)
                {
                    result.emplace_back(comp_a, comp_b);
                    ++i; ++j;
                }
                else if (ida < idb)
                {
                    ++i;
                }
                else
                {
                    ++j;
                }
            }
            return result;
        }

        template<typename A, typename B, typename... Rest>
        auto view_all() const
        {
            return join_all(view<A, B>(), get_components<Rest>()...);
        }

        template<typename A, typename Func>
        void each(Func func)
        {
            auto& components = get_components<A>();
            for(auto& comp : components)
            {
                func(comp);
            }
        }

        template<typename A, typename B, typename... Rest, typename Func>
        void each(Func func)
        {
            // запускаем внутренний имплементационный метод с индексовой последовательностью
            each_impl<A,B,Rest...>(std::index_sequence_for<A,B,Rest...>{},func);
        }

        template<typename A, typename B, typename... Rest, typename Cond, typename Func>
        void each_if(Cond cond, Func func)
        {
            // запускаем внутренний имплементационный метод с индексовой последовательностью
            each_if_impl<A,B,Rest...>(std::index_sequence_for<A,B,Rest...>{},func, cond);
        }

    private:
        //  Имплементация N-way merge через tuple<references> + индексы
        template<typename A, typename B, typename... Rest, size_t... Is, typename Func>
        void each_impl(std::index_sequence<Is...>, Func func)
        {
            // 1) Собираем tuple из ссылок на наши контейнеры
            auto containers = std::tie(get_components<A>(),
            get_components<B>(),
            get_components<Rest>()...);
            constexpr size_t N = 2 + sizeof...(Rest);

            // 2) Индексы по каждому вектору и их размеры
            std::array<size_t, N> idxs{};
            std::array<size_t, N> sizes
            {
                std::get<Is>(containers).size()...
            };

            // 3) N-way merge
            while (true)
            {
                // если любой индекс вышел за пределы — закончили
                if (((idxs[Is] >= sizes[Is]) || ...)) break;

                // читаем все текущие id
                std::array<int, N> ids
                {
                    std::get<Is>(containers)[idxs[Is]]->id...
                };

                // найдём максимальный id
                int max_id = ids[0];
                for (size_t k = 1; k < N; ++k)
                {
                    if (ids[k] > max_id) max_id = ids[k];
                }

                // если все id совпадают — вызовем func и сдвинем все индексы
                if (((ids[Is] == max_id) && ...))
                {
                    func(std::get<Is>(containers)[idxs[Is]]...);
                    (++idxs[Is], ...);
                }
                else
                {
                    // иначе продвигаем лишь те указатели, у которых id < max_id
                    for (size_t k = 0; k < N; ++k)
                    {
                        if (ids[k] < max_id)
                        {
                            ++idxs[k];
                        }
                    }
                }
            }
        }
        //  Имплементация N-way merge через tuple<references> + индексы
        template<typename A, typename B, typename... Rest, size_t... Is, typename Func, typename Cond>
        void each_if_impl(std::index_sequence<Is...>, Func func, Cond cond)
        {
            // 1) Собираем tuple из ссылок на наши контейнеры
            auto containers = std::tie(get_components<A>(),
            get_components<B>(),
            get_components<Rest>()...);
            constexpr size_t N = 2 + sizeof...(Rest);

            // 2) Индексы по каждому вектору и их размеры
            std::array<size_t, N> idxs{};
            std::array<size_t, N> sizes
            {
                std::get<Is>(containers).size()...
            };
            // 3) N-way merge
            while (true)
            {
                // если любой индекс вышел за пределы — закончили
                if (((idxs[Is] >= sizes[Is]) || ...)) break;

                // читаем все текущие id
                std::array<int, N> ids
                {
                    std::get<Is>(containers)[idxs[Is]]->id...
                };

                // найдём максимальный id
                int max_id = ids[0];
                for (size_t k = 1; k < N; ++k)
                {
                    if (ids[k] > max_id) max_id = ids[k];
                }

                // если все id совпадают — вызовем func и сдвинем все индексы
                if (((ids[Is] == max_id) && ...))
                {
                    if(cond(std::get<Is>(containers)[idxs[Is]]...))
                    func(std::get<Is>(containers)[idxs[Is]]...);
                    (++idxs[Is], ...);
                }
                else
                {
                    // иначе продвигаем лишь те указатели, у которых id < max_id
                    for (size_t k = 0; k < N; ++k)
                    {
                        if (ids[k] < max_id)
                        {
                            ++idxs[k];
                        }
                    }
                }
            }
        }

    };
} //namespace mg

#endif //#ifndef __mg_ModelEcsBase_h__
