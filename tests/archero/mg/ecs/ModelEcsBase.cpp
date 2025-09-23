#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../Observable.h"
#include "../model/ModelUser.h"
#include "ComponentActions.h"
#include "ComponentAura.h"
#include "ComponentAuraDamage.h"
#include "ComponentAuraOfSlow.h"
#include "ComponentBody.h"
#include "ComponentBullet.h"
#include "ComponentBulletFollowToTarget.h"
#include "ComponentBulletLaser.h"
#include "ComponentBulletSplit.h"
#include "ComponentBusy.h"
#include "ComponentCreateMovementToHero.h"
#include "ComponentCreateUnitsOnDeath.h"
#include "ComponentDamage.h"
#include "ComponentData.h"
#include "ComponentDropHeart.h"
#include "ComponentEffects.h"
#include "ComponentExp.h"
#include "ComponentExpDrop.h"
#include "ComponentGate.h"
#include "ComponentHealingLowHp.h"
#include "ComponentHealingOnChangeMaxHp.h"
#include "ComponentHealth.h"
#include "ComponentHeart.h"
#include "ComponentHeartAddStats.h"
#include "ComponentHeartAddStatsDamage.h"
#include "ComponentHeartAddStatsHp.h"
#include "ComponentLevelUp.h"
#include "ComponentMeleeAttack.h"
#include "ComponentMeteorPeriodic.h"
#include "ComponentMovement.h"
#include "ComponentPush.h"
#include "ComponentRandomHealing.h"
#include "ComponentRecharge.h"
#include "ComponentShield.h"
#include "ComponentShootBullet.h"
#include "ComponentSide.h"
#include "ComponentSpawn.h"
#include "ComponentSpawnSpirit.h"
#include "ComponentSphere.h"
#include "ComponentSphereSpawn.h"
#include "ComponentSpineInfo.h"
#include "ComponentSpirit.h"
#include "ComponentSpiritBasePoints.h"
#include "ComponentStats.h"
#include "ComponentStun.h"
#include "ComponentSword.h"
#include "ComponentSwordCast.h"
#include "ComponentTargetHighlight.h"
#include "ComponentTargetable.h"
#include "ComponentUser.h"
#include "ComponentVampire.h"
#include "DataLevel.h"
#include "DataStatUpgrade.h"
#include "ElectricDamage.h"
#include "FireDamage.h"
#include "Freezing.h"
#include "Ground.h"
#include "ModelEcsBase.h"
#include "MoveDirection.h"
#include "MoveInstant.h"
#include "MoveParabolic.h"
#include "MoveToTarget.h"
#include "MoveVertical.h"
#include "SystemActions.h"
#include "SystemAuraDamage.h"
#include "SystemAuroOfSlow.h"
#include "SystemBullet.h"
#include "SystemBulletClean.h"
#include "SystemBulletFollowToTarget.h"
#include "SystemCreateMovement.h"
#include "SystemCreateUnitsOnDeath.h"
#include "SystemDamage.h"
#include "SystemDeath.h"
#include "SystemDropHeart.h"
#include "SystemEffects.h"
#include "SystemExp.h"
#include "SystemFireDamage.h"
#include "SystemFreezing.h"
#include "SystemGate.h"
#include "SystemHealingOnChangeMaxHp.h"
#include "SystemMeleeAttack.h"
#include "SystemMeteor.h"
#include "SystemMovement.h"
#include "SystemPush.h"
#include "SystemRemoveDamage.h"
#include "SystemResolveCollisions.h"
#include "SystemShieldRestore.h"
#include "SystemShoot.h"
#include "SystemSpawn.h"
#include "SystemSphere.h"
#include "SystemSpiritMovement.h"
#include "SystemSpiritSpawn.h"
#include "SystemStun.h"
#include "SystemSwords.h"
#include "SystemSwordsCast.h"
#include "Task.h"
#include "Transform.h"
#include "Vector.h"
#include "intrusive_ptr.h"
#include <map>
#include <string>
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ModelEcsBase::TYPE("ModelEcsBase");

    ModelEcsBase::ModelEcsBase()
    : user(nullptr)
    , next_free_id(1)
    , player_id(0)
    , entities()
    , game_timer(0.0)
    , tasks()
    , data(nullptr)
    , wave_index(0)
    , ground(nullptr)
    , enemies_level(-1)
    , enemies_rank(-1)
    , wave_finished(false)
    , timer_wave_interval(0)
    , timer_wave_duration(0)
    , has_skills()
    , event_did_initialized()
    , event_wave_finish()
    , event_wave_start()
    , event_wave_next()
    , event_exit()
    , event_create_entity()
    , event_create_bullet()
    , event_remove_entity()
    , event_create_marker()
    , event_hero_level_up()
    , event_hero_exp_changed()
    , event_prepare_to_shoot()
    , event_turn_to()
    , event_shoot()
    , event_death()
    , event_skill_animate()
    , event_healing()
    , event_shield_activated()
    , event_dodge()
    , event_ressurection()
    , event_effects()
    , event_freeze_activated()
    , event_freeze_deactivated()
    , event_fire_activated()
    , event_fire_deactivated()
    , event_stun_activated()
    , event_stun_deactivated()
    , event_damaged()
    , event_aura_activated()
    , spawn_points()
    , _reference_counter(1)
    , components_data()
    , map_components_data()
    , components_transform()
    , map_components_transform()
    , components_stats()
    , map_components_stats()
    , components_side()
    , map_components_side()
    , components_targetable()
    , map_components_targetable()
    , components_health()
    , map_components_health()
    , components_damage()
    , map_components_damage()
    , components_user()
    , map_components_user()
    , components_spine_info()
    , map_components_spine_info()
    , components_busy()
    , map_components_busy()
    , components_create_units_on_death()
    , map_components_create_units_on_death()
    , components_aura()
    , map_components_aura()
    , components_aura_of_slow()
    , map_components_aura_of_slow()
    , components_aura_damage()
    , map_components_aura_damage()
    , components_spirit()
    , map_components_spirit()
    , components_spirit_base_points()
    , map_components_spirit_base_points()
    , components_spawn_spirit()
    , map_components_spawn_spirit()
    , components_recharge()
    , map_components_recharge()
    , components_bullet()
    , map_components_bullet()
    , components_bullet_laser()
    , map_components_bullet_laser()
    , components_shoot_bullet()
    , map_components_shoot_bullet()
    , components_melee_attack()
    , map_components_melee_attack()
    , components_target_highlight()
    , map_components_target_highlight()
    , components_bullet_split()
    , map_components_bullet_split()
    , components_shield()
    , map_components_shield()
    , components_effects()
    , map_components_effects()
    , components_bullet_follow_to_target()
    , map_components_bullet_follow_to_target()
    , components_meteor_periodic()
    , map_components_meteor_periodic()
    , components_electric_damage()
    , map_components_electric_damage()
    , components_sphere_spawn()
    , map_components_sphere_spawn()
    , components_sphere()
    , map_components_sphere()
    , components_actions()
    , map_components_actions()
    , components_gate()
    , map_components_gate()
    , components_level_up()
    , map_components_level_up()
    , components_exp()
    , map_components_exp()
    , components_exp_drop()
    , map_components_exp_drop()
    , components_stun()
    , map_components_stun()
    , components_fire_damage()
    , map_components_fire_damage()
    , components_movement()
    , map_components_movement()
    , components_move_direction()
    , map_components_move_direction()
    , components_move_to_target()
    , map_components_move_to_target()
    , components_move_instant()
    , map_components_move_instant()
    , components_move_vertical()
    , map_components_move_vertical()
    , components_freezing()
    , map_components_freezing()
    , components_move_parabolic()
    , map_components_move_parabolic()
    , components_create_movement_to_hero()
    , map_components_create_movement_to_hero()
    , components_push()
    , map_components_push()
    , components_spawn()
    , map_components_spawn()
    , components_healing_on_change_max_hp()
    , map_components_healing_on_change_max_hp()
    , components_drop_heart()
    , map_components_drop_heart()
    , components_heart()
    , map_components_heart()
    , components_heart_add_stats()
    , map_components_heart_add_stats()
    , components_heart_add_stats_hp()
    , map_components_heart_add_stats_hp()
    , components_heart_add_stats_damage()
    , map_components_heart_add_stats_damage()
    , components_random_healing()
    , map_components_random_healing()
    , components_vampire()
    , map_components_vampire()
    , components_healing_low_hp()
    , map_components_healing_low_hp()
    , components_body()
    , map_components_body()
    , components_sword_cast()
    , map_components_sword_cast()
    , components_sword()
    , map_components_sword()
    {

    }

    ModelEcsBase::~ModelEcsBase()
    {
    }

    template <> std::vector<intrusive_ptr<ComponentData>>& ModelEcsBase::get_components()
    {
        return this->components_data;
    }
    template <> std::vector<intrusive_ptr<Transform>>& ModelEcsBase::get_components()
    {
        return this->components_transform;
    }
    template <> std::vector<intrusive_ptr<ComponentStats>>& ModelEcsBase::get_components()
    {
        return this->components_stats;
    }
    template <> std::vector<intrusive_ptr<ComponentSide>>& ModelEcsBase::get_components()
    {
        return this->components_side;
    }
    template <> std::vector<intrusive_ptr<ComponentTargetable>>& ModelEcsBase::get_components()
    {
        return this->components_targetable;
    }
    template <> std::vector<intrusive_ptr<ComponentHealth>>& ModelEcsBase::get_components()
    {
        return this->components_health;
    }
    template <> std::vector<intrusive_ptr<ComponentDamage>>& ModelEcsBase::get_components()
    {
        return this->components_damage;
    }
    template <> std::vector<intrusive_ptr<ComponentUser>>& ModelEcsBase::get_components()
    {
        return this->components_user;
    }
    template <> std::vector<intrusive_ptr<ComponentSpineInfo>>& ModelEcsBase::get_components()
    {
        return this->components_spine_info;
    }
    template <> std::vector<intrusive_ptr<ComponentBusy>>& ModelEcsBase::get_components()
    {
        return this->components_busy;
    }
    template <> std::vector<intrusive_ptr<ComponentCreateUnitsOnDeath>>& ModelEcsBase::get_components()
    {
        return this->components_create_units_on_death;
    }
    template <> std::vector<intrusive_ptr<ComponentAura>>& ModelEcsBase::get_components()
    {
        return this->components_aura;
    }
    template <> std::vector<intrusive_ptr<ComponentAuraOfSlow>>& ModelEcsBase::get_components()
    {
        return this->components_aura_of_slow;
    }
    template <> std::vector<intrusive_ptr<ComponentAuraDamage>>& ModelEcsBase::get_components()
    {
        return this->components_aura_damage;
    }
    template <> std::vector<intrusive_ptr<ComponentSpirit>>& ModelEcsBase::get_components()
    {
        return this->components_spirit;
    }
    template <> std::vector<intrusive_ptr<ComponentSpiritBasePoints>>& ModelEcsBase::get_components()
    {
        return this->components_spirit_base_points;
    }
    template <> std::vector<intrusive_ptr<ComponentSpawnSpirit>>& ModelEcsBase::get_components()
    {
        return this->components_spawn_spirit;
    }
    template <> std::vector<intrusive_ptr<ComponentRecharge>>& ModelEcsBase::get_components()
    {
        return this->components_recharge;
    }
    template <> std::vector<intrusive_ptr<ComponentBullet>>& ModelEcsBase::get_components()
    {
        return this->components_bullet;
    }
    template <> std::vector<intrusive_ptr<ComponentBulletLaser>>& ModelEcsBase::get_components()
    {
        return this->components_bullet_laser;
    }
    template <> std::vector<intrusive_ptr<ComponentShootBullet>>& ModelEcsBase::get_components()
    {
        return this->components_shoot_bullet;
    }
    template <> std::vector<intrusive_ptr<ComponentMeleeAttack>>& ModelEcsBase::get_components()
    {
        return this->components_melee_attack;
    }
    template <> std::vector<intrusive_ptr<ComponentTargetHighlight>>& ModelEcsBase::get_components()
    {
        return this->components_target_highlight;
    }
    template <> std::vector<intrusive_ptr<ComponentBulletSplit>>& ModelEcsBase::get_components()
    {
        return this->components_bullet_split;
    }
    template <> std::vector<intrusive_ptr<ComponentShield>>& ModelEcsBase::get_components()
    {
        return this->components_shield;
    }
    template <> std::vector<intrusive_ptr<ComponentEffects>>& ModelEcsBase::get_components()
    {
        return this->components_effects;
    }
    template <> std::vector<intrusive_ptr<ComponentBulletFollowToTarget>>& ModelEcsBase::get_components()
    {
        return this->components_bullet_follow_to_target;
    }
    template <> std::vector<intrusive_ptr<ComponentMeteorPeriodic>>& ModelEcsBase::get_components()
    {
        return this->components_meteor_periodic;
    }
    template <> std::vector<intrusive_ptr<ElectricDamage>>& ModelEcsBase::get_components()
    {
        return this->components_electric_damage;
    }
    template <> std::vector<intrusive_ptr<ComponentSphereSpawn>>& ModelEcsBase::get_components()
    {
        return this->components_sphere_spawn;
    }
    template <> std::vector<intrusive_ptr<ComponentSphere>>& ModelEcsBase::get_components()
    {
        return this->components_sphere;
    }
    template <> std::vector<intrusive_ptr<ComponentActions>>& ModelEcsBase::get_components()
    {
        return this->components_actions;
    }
    template <> std::vector<intrusive_ptr<ComponentGate>>& ModelEcsBase::get_components()
    {
        return this->components_gate;
    }
    template <> std::vector<intrusive_ptr<ComponentLevelUp>>& ModelEcsBase::get_components()
    {
        return this->components_level_up;
    }
    template <> std::vector<intrusive_ptr<ComponentExp>>& ModelEcsBase::get_components()
    {
        return this->components_exp;
    }
    template <> std::vector<intrusive_ptr<ComponentExpDrop>>& ModelEcsBase::get_components()
    {
        return this->components_exp_drop;
    }
    template <> std::vector<intrusive_ptr<ComponentStun>>& ModelEcsBase::get_components()
    {
        return this->components_stun;
    }
    template <> std::vector<intrusive_ptr<FireDamage>>& ModelEcsBase::get_components()
    {
        return this->components_fire_damage;
    }
    template <> std::vector<intrusive_ptr<ComponentMovement>>& ModelEcsBase::get_components()
    {
        return this->components_movement;
    }
    template <> std::vector<intrusive_ptr<MoveDirection>>& ModelEcsBase::get_components()
    {
        return this->components_move_direction;
    }
    template <> std::vector<intrusive_ptr<MoveToTarget>>& ModelEcsBase::get_components()
    {
        return this->components_move_to_target;
    }
    template <> std::vector<intrusive_ptr<MoveInstant>>& ModelEcsBase::get_components()
    {
        return this->components_move_instant;
    }
    template <> std::vector<intrusive_ptr<MoveVertical>>& ModelEcsBase::get_components()
    {
        return this->components_move_vertical;
    }
    template <> std::vector<intrusive_ptr<Freezing>>& ModelEcsBase::get_components()
    {
        return this->components_freezing;
    }
    template <> std::vector<intrusive_ptr<MoveParabolic>>& ModelEcsBase::get_components()
    {
        return this->components_move_parabolic;
    }
    template <> std::vector<intrusive_ptr<ComponentCreateMovementToHero>>& ModelEcsBase::get_components()
    {
        return this->components_create_movement_to_hero;
    }
    template <> std::vector<intrusive_ptr<ComponentPush>>& ModelEcsBase::get_components()
    {
        return this->components_push;
    }
    template <> std::vector<intrusive_ptr<ComponentSpawn>>& ModelEcsBase::get_components()
    {
        return this->components_spawn;
    }
    template <> std::vector<intrusive_ptr<ComponentHealingOnChangeMaxHp>>& ModelEcsBase::get_components()
    {
        return this->components_healing_on_change_max_hp;
    }
    template <> std::vector<intrusive_ptr<ComponentDropHeart>>& ModelEcsBase::get_components()
    {
        return this->components_drop_heart;
    }
    template <> std::vector<intrusive_ptr<ComponentHeart>>& ModelEcsBase::get_components()
    {
        return this->components_heart;
    }
    template <> std::vector<intrusive_ptr<ComponentHeartAddStats>>& ModelEcsBase::get_components()
    {
        return this->components_heart_add_stats;
    }
    template <> std::vector<intrusive_ptr<ComponentHeartAddStatsHp>>& ModelEcsBase::get_components()
    {
        return this->components_heart_add_stats_hp;
    }
    template <> std::vector<intrusive_ptr<ComponentHeartAddStatsDamage>>& ModelEcsBase::get_components()
    {
        return this->components_heart_add_stats_damage;
    }
    template <> std::vector<intrusive_ptr<ComponentRandomHealing>>& ModelEcsBase::get_components()
    {
        return this->components_random_healing;
    }
    template <> std::vector<intrusive_ptr<ComponentVampire>>& ModelEcsBase::get_components()
    {
        return this->components_vampire;
    }
    template <> std::vector<intrusive_ptr<ComponentHealingLowHp>>& ModelEcsBase::get_components()
    {
        return this->components_healing_low_hp;
    }
    template <> std::vector<intrusive_ptr<ComponentBody>>& ModelEcsBase::get_components()
    {
        return this->components_body;
    }
    template <> std::vector<intrusive_ptr<ComponentSwordCast>>& ModelEcsBase::get_components()
    {
        return this->components_sword_cast;
    }
    template <> std::vector<intrusive_ptr<ComponentSword>>& ModelEcsBase::get_components()
    {
        return this->components_sword;
    }
    template <> const std::vector<intrusive_ptr<ComponentData>>& ModelEcsBase::get_components() const
    {
        return this->components_data;
    }
    template <> const std::vector<intrusive_ptr<Transform>>& ModelEcsBase::get_components() const
    {
        return this->components_transform;
    }
    template <> const std::vector<intrusive_ptr<ComponentStats>>& ModelEcsBase::get_components() const
    {
        return this->components_stats;
    }
    template <> const std::vector<intrusive_ptr<ComponentSide>>& ModelEcsBase::get_components() const
    {
        return this->components_side;
    }
    template <> const std::vector<intrusive_ptr<ComponentTargetable>>& ModelEcsBase::get_components() const
    {
        return this->components_targetable;
    }
    template <> const std::vector<intrusive_ptr<ComponentHealth>>& ModelEcsBase::get_components() const
    {
        return this->components_health;
    }
    template <> const std::vector<intrusive_ptr<ComponentDamage>>& ModelEcsBase::get_components() const
    {
        return this->components_damage;
    }
    template <> const std::vector<intrusive_ptr<ComponentUser>>& ModelEcsBase::get_components() const
    {
        return this->components_user;
    }
    template <> const std::vector<intrusive_ptr<ComponentSpineInfo>>& ModelEcsBase::get_components() const
    {
        return this->components_spine_info;
    }
    template <> const std::vector<intrusive_ptr<ComponentBusy>>& ModelEcsBase::get_components() const
    {
        return this->components_busy;
    }
    template <> const std::vector<intrusive_ptr<ComponentCreateUnitsOnDeath>>& ModelEcsBase::get_components() const
    {
        return this->components_create_units_on_death;
    }
    template <> const std::vector<intrusive_ptr<ComponentAura>>& ModelEcsBase::get_components() const
    {
        return this->components_aura;
    }
    template <> const std::vector<intrusive_ptr<ComponentAuraOfSlow>>& ModelEcsBase::get_components() const
    {
        return this->components_aura_of_slow;
    }
    template <> const std::vector<intrusive_ptr<ComponentAuraDamage>>& ModelEcsBase::get_components() const
    {
        return this->components_aura_damage;
    }
    template <> const std::vector<intrusive_ptr<ComponentSpirit>>& ModelEcsBase::get_components() const
    {
        return this->components_spirit;
    }
    template <> const std::vector<intrusive_ptr<ComponentSpiritBasePoints>>& ModelEcsBase::get_components() const
    {
        return this->components_spirit_base_points;
    }
    template <> const std::vector<intrusive_ptr<ComponentSpawnSpirit>>& ModelEcsBase::get_components() const
    {
        return this->components_spawn_spirit;
    }
    template <> const std::vector<intrusive_ptr<ComponentRecharge>>& ModelEcsBase::get_components() const
    {
        return this->components_recharge;
    }
    template <> const std::vector<intrusive_ptr<ComponentBullet>>& ModelEcsBase::get_components() const
    {
        return this->components_bullet;
    }
    template <> const std::vector<intrusive_ptr<ComponentBulletLaser>>& ModelEcsBase::get_components() const
    {
        return this->components_bullet_laser;
    }
    template <> const std::vector<intrusive_ptr<ComponentShootBullet>>& ModelEcsBase::get_components() const
    {
        return this->components_shoot_bullet;
    }
    template <> const std::vector<intrusive_ptr<ComponentMeleeAttack>>& ModelEcsBase::get_components() const
    {
        return this->components_melee_attack;
    }
    template <> const std::vector<intrusive_ptr<ComponentTargetHighlight>>& ModelEcsBase::get_components() const
    {
        return this->components_target_highlight;
    }
    template <> const std::vector<intrusive_ptr<ComponentBulletSplit>>& ModelEcsBase::get_components() const
    {
        return this->components_bullet_split;
    }
    template <> const std::vector<intrusive_ptr<ComponentShield>>& ModelEcsBase::get_components() const
    {
        return this->components_shield;
    }
    template <> const std::vector<intrusive_ptr<ComponentEffects>>& ModelEcsBase::get_components() const
    {
        return this->components_effects;
    }
    template <> const std::vector<intrusive_ptr<ComponentBulletFollowToTarget>>& ModelEcsBase::get_components() const
    {
        return this->components_bullet_follow_to_target;
    }
    template <> const std::vector<intrusive_ptr<ComponentMeteorPeriodic>>& ModelEcsBase::get_components() const
    {
        return this->components_meteor_periodic;
    }
    template <> const std::vector<intrusive_ptr<ElectricDamage>>& ModelEcsBase::get_components() const
    {
        return this->components_electric_damage;
    }
    template <> const std::vector<intrusive_ptr<ComponentSphereSpawn>>& ModelEcsBase::get_components() const
    {
        return this->components_sphere_spawn;
    }
    template <> const std::vector<intrusive_ptr<ComponentSphere>>& ModelEcsBase::get_components() const
    {
        return this->components_sphere;
    }
    template <> const std::vector<intrusive_ptr<ComponentActions>>& ModelEcsBase::get_components() const
    {
        return this->components_actions;
    }
    template <> const std::vector<intrusive_ptr<ComponentGate>>& ModelEcsBase::get_components() const
    {
        return this->components_gate;
    }
    template <> const std::vector<intrusive_ptr<ComponentLevelUp>>& ModelEcsBase::get_components() const
    {
        return this->components_level_up;
    }
    template <> const std::vector<intrusive_ptr<ComponentExp>>& ModelEcsBase::get_components() const
    {
        return this->components_exp;
    }
    template <> const std::vector<intrusive_ptr<ComponentExpDrop>>& ModelEcsBase::get_components() const
    {
        return this->components_exp_drop;
    }
    template <> const std::vector<intrusive_ptr<ComponentStun>>& ModelEcsBase::get_components() const
    {
        return this->components_stun;
    }
    template <> const std::vector<intrusive_ptr<FireDamage>>& ModelEcsBase::get_components() const
    {
        return this->components_fire_damage;
    }
    template <> const std::vector<intrusive_ptr<ComponentMovement>>& ModelEcsBase::get_components() const
    {
        return this->components_movement;
    }
    template <> const std::vector<intrusive_ptr<MoveDirection>>& ModelEcsBase::get_components() const
    {
        return this->components_move_direction;
    }
    template <> const std::vector<intrusive_ptr<MoveToTarget>>& ModelEcsBase::get_components() const
    {
        return this->components_move_to_target;
    }
    template <> const std::vector<intrusive_ptr<MoveInstant>>& ModelEcsBase::get_components() const
    {
        return this->components_move_instant;
    }
    template <> const std::vector<intrusive_ptr<MoveVertical>>& ModelEcsBase::get_components() const
    {
        return this->components_move_vertical;
    }
    template <> const std::vector<intrusive_ptr<Freezing>>& ModelEcsBase::get_components() const
    {
        return this->components_freezing;
    }
    template <> const std::vector<intrusive_ptr<MoveParabolic>>& ModelEcsBase::get_components() const
    {
        return this->components_move_parabolic;
    }
    template <> const std::vector<intrusive_ptr<ComponentCreateMovementToHero>>& ModelEcsBase::get_components() const
    {
        return this->components_create_movement_to_hero;
    }
    template <> const std::vector<intrusive_ptr<ComponentPush>>& ModelEcsBase::get_components() const
    {
        return this->components_push;
    }
    template <> const std::vector<intrusive_ptr<ComponentSpawn>>& ModelEcsBase::get_components() const
    {
        return this->components_spawn;
    }
    template <> const std::vector<intrusive_ptr<ComponentHealingOnChangeMaxHp>>& ModelEcsBase::get_components() const
    {
        return this->components_healing_on_change_max_hp;
    }
    template <> const std::vector<intrusive_ptr<ComponentDropHeart>>& ModelEcsBase::get_components() const
    {
        return this->components_drop_heart;
    }
    template <> const std::vector<intrusive_ptr<ComponentHeart>>& ModelEcsBase::get_components() const
    {
        return this->components_heart;
    }
    template <> const std::vector<intrusive_ptr<ComponentHeartAddStats>>& ModelEcsBase::get_components() const
    {
        return this->components_heart_add_stats;
    }
    template <> const std::vector<intrusive_ptr<ComponentHeartAddStatsHp>>& ModelEcsBase::get_components() const
    {
        return this->components_heart_add_stats_hp;
    }
    template <> const std::vector<intrusive_ptr<ComponentHeartAddStatsDamage>>& ModelEcsBase::get_components() const
    {
        return this->components_heart_add_stats_damage;
    }
    template <> const std::vector<intrusive_ptr<ComponentRandomHealing>>& ModelEcsBase::get_components() const
    {
        return this->components_random_healing;
    }
    template <> const std::vector<intrusive_ptr<ComponentVampire>>& ModelEcsBase::get_components() const
    {
        return this->components_vampire;
    }
    template <> const std::vector<intrusive_ptr<ComponentHealingLowHp>>& ModelEcsBase::get_components() const
    {
        return this->components_healing_low_hp;
    }
    template <> const std::vector<intrusive_ptr<ComponentBody>>& ModelEcsBase::get_components() const
    {
        return this->components_body;
    }
    template <> const std::vector<intrusive_ptr<ComponentSwordCast>>& ModelEcsBase::get_components() const
    {
        return this->components_sword_cast;
    }
    template <> const std::vector<intrusive_ptr<ComponentSword>>& ModelEcsBase::get_components() const
    {
        return this->components_sword;
    }
    template<> intrusive_ptr<ComponentData> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_data))
        {
            return this->map_components_data.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<Transform> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_transform))
        {
            return this->map_components_transform.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentStats> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_stats))
        {
            return this->map_components_stats.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSide> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_side))
        {
            return this->map_components_side.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentTargetable> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_targetable))
        {
            return this->map_components_targetable.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentHealth> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_health))
        {
            return this->map_components_health.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentDamage> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_damage))
        {
            return this->map_components_damage.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentUser> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_user))
        {
            return this->map_components_user.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSpineInfo> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_spine_info))
        {
            return this->map_components_spine_info.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentBusy> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_busy))
        {
            return this->map_components_busy.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentCreateUnitsOnDeath> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_create_units_on_death))
        {
            return this->map_components_create_units_on_death.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentAura> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_aura))
        {
            return this->map_components_aura.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentAuraOfSlow> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_aura_of_slow))
        {
            return this->map_components_aura_of_slow.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentAuraDamage> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_aura_damage))
        {
            return this->map_components_aura_damage.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSpirit> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_spirit))
        {
            return this->map_components_spirit.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSpiritBasePoints> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_spirit_base_points))
        {
            return this->map_components_spirit_base_points.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSpawnSpirit> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_spawn_spirit))
        {
            return this->map_components_spawn_spirit.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentRecharge> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_recharge))
        {
            return this->map_components_recharge.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentBullet> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_bullet))
        {
            return this->map_components_bullet.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentBulletLaser> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_bullet_laser))
        {
            return this->map_components_bullet_laser.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentShootBullet> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_shoot_bullet))
        {
            return this->map_components_shoot_bullet.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentMeleeAttack> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_melee_attack))
        {
            return this->map_components_melee_attack.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentTargetHighlight> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_target_highlight))
        {
            return this->map_components_target_highlight.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentBulletSplit> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_bullet_split))
        {
            return this->map_components_bullet_split.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentShield> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_shield))
        {
            return this->map_components_shield.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentEffects> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_effects))
        {
            return this->map_components_effects.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentBulletFollowToTarget> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_bullet_follow_to_target))
        {
            return this->map_components_bullet_follow_to_target.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentMeteorPeriodic> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_meteor_periodic))
        {
            return this->map_components_meteor_periodic.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ElectricDamage> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_electric_damage))
        {
            return this->map_components_electric_damage.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSphereSpawn> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_sphere_spawn))
        {
            return this->map_components_sphere_spawn.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSphere> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_sphere))
        {
            return this->map_components_sphere.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentActions> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_actions))
        {
            return this->map_components_actions.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentGate> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_gate))
        {
            return this->map_components_gate.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentLevelUp> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_level_up))
        {
            return this->map_components_level_up.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentExp> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_exp))
        {
            return this->map_components_exp.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentExpDrop> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_exp_drop))
        {
            return this->map_components_exp_drop.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentStun> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_stun))
        {
            return this->map_components_stun.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<FireDamage> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_fire_damage))
        {
            return this->map_components_fire_damage.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentMovement> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_movement))
        {
            return this->map_components_movement.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<MoveDirection> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_move_direction))
        {
            return this->map_components_move_direction.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<MoveToTarget> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_move_to_target))
        {
            return this->map_components_move_to_target.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<MoveInstant> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_move_instant))
        {
            return this->map_components_move_instant.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<MoveVertical> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_move_vertical))
        {
            return this->map_components_move_vertical.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<Freezing> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_freezing))
        {
            return this->map_components_freezing.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<MoveParabolic> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_move_parabolic))
        {
            return this->map_components_move_parabolic.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentCreateMovementToHero> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_create_movement_to_hero))
        {
            return this->map_components_create_movement_to_hero.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentPush> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_push))
        {
            return this->map_components_push.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSpawn> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_spawn))
        {
            return this->map_components_spawn.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentHealingOnChangeMaxHp> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_healing_on_change_max_hp))
        {
            return this->map_components_healing_on_change_max_hp.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentDropHeart> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_drop_heart))
        {
            return this->map_components_drop_heart.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentHeart> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_heart))
        {
            return this->map_components_heart.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentHeartAddStats> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_heart_add_stats))
        {
            return this->map_components_heart_add_stats.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentHeartAddStatsHp> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_heart_add_stats_hp))
        {
            return this->map_components_heart_add_stats_hp.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentHeartAddStatsDamage> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_heart_add_stats_damage))
        {
            return this->map_components_heart_add_stats_damage.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentRandomHealing> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_random_healing))
        {
            return this->map_components_random_healing.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentVampire> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_vampire))
        {
            return this->map_components_vampire.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentHealingLowHp> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_healing_low_hp))
        {
            return this->map_components_healing_low_hp.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentBody> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_body))
        {
            return this->map_components_body.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSwordCast> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_sword_cast))
        {
            return this->map_components_sword_cast.at(component_id);
        }
        return nullptr;
    }

    template<> intrusive_ptr<ComponentSword> ModelEcsBase::get(int component_id)
    {
        if(in_map(component_id, this->map_components_sword))
        {
            return this->map_components_sword.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentData* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_data))
        {
            return this->map_components_data.at(component_id);
        }
        return nullptr;
    }

    template<> const Transform* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_transform))
        {
            return this->map_components_transform.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentStats* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_stats))
        {
            return this->map_components_stats.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSide* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_side))
        {
            return this->map_components_side.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentTargetable* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_targetable))
        {
            return this->map_components_targetable.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentHealth* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_health))
        {
            return this->map_components_health.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentDamage* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_damage))
        {
            return this->map_components_damage.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentUser* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_user))
        {
            return this->map_components_user.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSpineInfo* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_spine_info))
        {
            return this->map_components_spine_info.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentBusy* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_busy))
        {
            return this->map_components_busy.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentCreateUnitsOnDeath* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_create_units_on_death))
        {
            return this->map_components_create_units_on_death.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentAura* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_aura))
        {
            return this->map_components_aura.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentAuraOfSlow* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_aura_of_slow))
        {
            return this->map_components_aura_of_slow.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentAuraDamage* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_aura_damage))
        {
            return this->map_components_aura_damage.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSpirit* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_spirit))
        {
            return this->map_components_spirit.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSpiritBasePoints* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_spirit_base_points))
        {
            return this->map_components_spirit_base_points.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSpawnSpirit* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_spawn_spirit))
        {
            return this->map_components_spawn_spirit.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentRecharge* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_recharge))
        {
            return this->map_components_recharge.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentBullet* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_bullet))
        {
            return this->map_components_bullet.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentBulletLaser* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_bullet_laser))
        {
            return this->map_components_bullet_laser.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentShootBullet* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_shoot_bullet))
        {
            return this->map_components_shoot_bullet.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentMeleeAttack* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_melee_attack))
        {
            return this->map_components_melee_attack.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentTargetHighlight* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_target_highlight))
        {
            return this->map_components_target_highlight.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentBulletSplit* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_bullet_split))
        {
            return this->map_components_bullet_split.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentShield* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_shield))
        {
            return this->map_components_shield.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentEffects* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_effects))
        {
            return this->map_components_effects.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentBulletFollowToTarget* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_bullet_follow_to_target))
        {
            return this->map_components_bullet_follow_to_target.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentMeteorPeriodic* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_meteor_periodic))
        {
            return this->map_components_meteor_periodic.at(component_id);
        }
        return nullptr;
    }

    template<> const ElectricDamage* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_electric_damage))
        {
            return this->map_components_electric_damage.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSphereSpawn* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_sphere_spawn))
        {
            return this->map_components_sphere_spawn.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSphere* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_sphere))
        {
            return this->map_components_sphere.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentActions* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_actions))
        {
            return this->map_components_actions.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentGate* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_gate))
        {
            return this->map_components_gate.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentLevelUp* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_level_up))
        {
            return this->map_components_level_up.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentExp* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_exp))
        {
            return this->map_components_exp.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentExpDrop* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_exp_drop))
        {
            return this->map_components_exp_drop.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentStun* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_stun))
        {
            return this->map_components_stun.at(component_id);
        }
        return nullptr;
    }

    template<> const FireDamage* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_fire_damage))
        {
            return this->map_components_fire_damage.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentMovement* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_movement))
        {
            return this->map_components_movement.at(component_id);
        }
        return nullptr;
    }

    template<> const MoveDirection* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_move_direction))
        {
            return this->map_components_move_direction.at(component_id);
        }
        return nullptr;
    }

    template<> const MoveToTarget* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_move_to_target))
        {
            return this->map_components_move_to_target.at(component_id);
        }
        return nullptr;
    }

    template<> const MoveInstant* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_move_instant))
        {
            return this->map_components_move_instant.at(component_id);
        }
        return nullptr;
    }

    template<> const MoveVertical* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_move_vertical))
        {
            return this->map_components_move_vertical.at(component_id);
        }
        return nullptr;
    }

    template<> const Freezing* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_freezing))
        {
            return this->map_components_freezing.at(component_id);
        }
        return nullptr;
    }

    template<> const MoveParabolic* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_move_parabolic))
        {
            return this->map_components_move_parabolic.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentCreateMovementToHero* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_create_movement_to_hero))
        {
            return this->map_components_create_movement_to_hero.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentPush* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_push))
        {
            return this->map_components_push.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSpawn* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_spawn))
        {
            return this->map_components_spawn.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentHealingOnChangeMaxHp* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_healing_on_change_max_hp))
        {
            return this->map_components_healing_on_change_max_hp.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentDropHeart* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_drop_heart))
        {
            return this->map_components_drop_heart.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentHeart* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_heart))
        {
            return this->map_components_heart.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentHeartAddStats* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_heart_add_stats))
        {
            return this->map_components_heart_add_stats.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentHeartAddStatsHp* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_heart_add_stats_hp))
        {
            return this->map_components_heart_add_stats_hp.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentHeartAddStatsDamage* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_heart_add_stats_damage))
        {
            return this->map_components_heart_add_stats_damage.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentRandomHealing* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_random_healing))
        {
            return this->map_components_random_healing.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentVampire* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_vampire))
        {
            return this->map_components_vampire.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentHealingLowHp* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_healing_low_hp))
        {
            return this->map_components_healing_low_hp.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentBody* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_body))
        {
            return this->map_components_body.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSwordCast* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_sword_cast))
        {
            return this->map_components_sword_cast.at(component_id);
        }
        return nullptr;
    }

    template<> const ComponentSword* ModelEcsBase::get(int component_id) const
    {
        if(in_map(component_id, this->map_components_sword))
        {
            return this->map_components_sword.at(component_id);
        }
        return nullptr;
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentData> component)
    {
        if(component)
        {
            list_remove(this->components_data, component); map_remove(this->map_components_data, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<Transform> component)
    {
        if(component)
        {
            list_remove(this->components_transform, component); map_remove(this->map_components_transform, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentStats> component)
    {
        if(component)
        {
            list_remove(this->components_stats, component); map_remove(this->map_components_stats, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSide> component)
    {
        if(component)
        {
            list_remove(this->components_side, component); map_remove(this->map_components_side, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentTargetable> component)
    {
        if(component)
        {
            list_remove(this->components_targetable, component); map_remove(this->map_components_targetable, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentHealth> component)
    {
        if(component)
        {
            list_remove(this->components_health, component); map_remove(this->map_components_health, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentDamage> component)
    {
        if(component)
        {
            list_remove(this->components_damage, component); map_remove(this->map_components_damage, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentUser> component)
    {
        if(component)
        {
            list_remove(this->components_user, component); map_remove(this->map_components_user, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSpineInfo> component)
    {
        if(component)
        {
            list_remove(this->components_spine_info, component); map_remove(this->map_components_spine_info, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentBusy> component)
    {
        if(component)
        {
            list_remove(this->components_busy, component); map_remove(this->map_components_busy, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentCreateUnitsOnDeath> component)
    {
        if(component)
        {
            list_remove(this->components_create_units_on_death, component); map_remove(this->map_components_create_units_on_death, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentAura> component)
    {
        if(component)
        {
            list_remove(this->components_aura, component); map_remove(this->map_components_aura, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentAuraOfSlow> component)
    {
        this->remove<ComponentAura>(intrusive_ptr<ComponentAura>(component));
        if(component)
        {
            list_remove(this->components_aura_of_slow, component); map_remove(this->map_components_aura_of_slow, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentAuraDamage> component)
    {
        this->remove<ComponentAura>(intrusive_ptr<ComponentAura>(component));
        if(component)
        {
            list_remove(this->components_aura_damage, component); map_remove(this->map_components_aura_damage, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSpirit> component)
    {
        if(component)
        {
            list_remove(this->components_spirit, component); map_remove(this->map_components_spirit, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSpiritBasePoints> component)
    {
        if(component)
        {
            list_remove(this->components_spirit_base_points, component); map_remove(this->map_components_spirit_base_points, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSpawnSpirit> component)
    {
        if(component)
        {
            list_remove(this->components_spawn_spirit, component); map_remove(this->map_components_spawn_spirit, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentRecharge> component)
    {
        if(component)
        {
            list_remove(this->components_recharge, component); map_remove(this->map_components_recharge, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentBullet> component)
    {
        if(component)
        {
            list_remove(this->components_bullet, component); map_remove(this->map_components_bullet, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentBulletLaser> component)
    {
        this->remove<ComponentBullet>(intrusive_ptr<ComponentBullet>(component));
        if(component)
        {
            list_remove(this->components_bullet_laser, component); map_remove(this->map_components_bullet_laser, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentShootBullet> component)
    {
        if(component)
        {
            list_remove(this->components_shoot_bullet, component); map_remove(this->map_components_shoot_bullet, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentMeleeAttack> component)
    {
        if(component)
        {
            list_remove(this->components_melee_attack, component); map_remove(this->map_components_melee_attack, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentTargetHighlight> component)
    {
        if(component)
        {
            list_remove(this->components_target_highlight, component); map_remove(this->map_components_target_highlight, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentBulletSplit> component)
    {
        if(component)
        {
            list_remove(this->components_bullet_split, component); map_remove(this->map_components_bullet_split, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentShield> component)
    {
        if(component)
        {
            list_remove(this->components_shield, component); map_remove(this->map_components_shield, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentEffects> component)
    {
        if(component)
        {
            list_remove(this->components_effects, component); map_remove(this->map_components_effects, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentBulletFollowToTarget> component)
    {
        if(component)
        {
            list_remove(this->components_bullet_follow_to_target, component); map_remove(this->map_components_bullet_follow_to_target, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentMeteorPeriodic> component)
    {
        if(component)
        {
            list_remove(this->components_meteor_periodic, component); map_remove(this->map_components_meteor_periodic, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ElectricDamage> component)
    {
        if(component)
        {
            list_remove(this->components_electric_damage, component); map_remove(this->map_components_electric_damage, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSphereSpawn> component)
    {
        if(component)
        {
            list_remove(this->components_sphere_spawn, component); map_remove(this->map_components_sphere_spawn, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSphere> component)
    {
        if(component)
        {
            list_remove(this->components_sphere, component); map_remove(this->map_components_sphere, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentActions> component)
    {
        if(component)
        {
            list_remove(this->components_actions, component); map_remove(this->map_components_actions, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentGate> component)
    {
        if(component)
        {
            list_remove(this->components_gate, component); map_remove(this->map_components_gate, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentLevelUp> component)
    {
        if(component)
        {
            list_remove(this->components_level_up, component); map_remove(this->map_components_level_up, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentExp> component)
    {
        if(component)
        {
            list_remove(this->components_exp, component); map_remove(this->map_components_exp, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentExpDrop> component)
    {
        if(component)
        {
            list_remove(this->components_exp_drop, component); map_remove(this->map_components_exp_drop, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentStun> component)
    {
        if(component)
        {
            list_remove(this->components_stun, component); map_remove(this->map_components_stun, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<FireDamage> component)
    {
        if(component)
        {
            list_remove(this->components_fire_damage, component); map_remove(this->map_components_fire_damage, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentMovement> component)
    {
        if(component)
        {
            list_remove(this->components_movement, component); map_remove(this->map_components_movement, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<MoveDirection> component)
    {
        this->remove<ComponentMovement>(intrusive_ptr<ComponentMovement>(component));
        if(component)
        {
            list_remove(this->components_move_direction, component); map_remove(this->map_components_move_direction, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<MoveToTarget> component)
    {
        this->remove<ComponentMovement>(intrusive_ptr<ComponentMovement>(component));
        if(component)
        {
            list_remove(this->components_move_to_target, component); map_remove(this->map_components_move_to_target, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<MoveInstant> component)
    {
        this->remove<ComponentMovement>(intrusive_ptr<ComponentMovement>(component));
        if(component)
        {
            list_remove(this->components_move_instant, component); map_remove(this->map_components_move_instant, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<MoveVertical> component)
    {
        this->remove<ComponentMovement>(intrusive_ptr<ComponentMovement>(component));
        if(component)
        {
            list_remove(this->components_move_vertical, component); map_remove(this->map_components_move_vertical, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<Freezing> component)
    {
        if(component)
        {
            list_remove(this->components_freezing, component); map_remove(this->map_components_freezing, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<MoveParabolic> component)
    {
        if(component)
        {
            list_remove(this->components_move_parabolic, component); map_remove(this->map_components_move_parabolic, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentCreateMovementToHero> component)
    {
        if(component)
        {
            list_remove(this->components_create_movement_to_hero, component); map_remove(this->map_components_create_movement_to_hero, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentPush> component)
    {
        if(component)
        {
            list_remove(this->components_push, component); map_remove(this->map_components_push, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSpawn> component)
    {
        if(component)
        {
            list_remove(this->components_spawn, component); map_remove(this->map_components_spawn, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentHealingOnChangeMaxHp> component)
    {
        if(component)
        {
            list_remove(this->components_healing_on_change_max_hp, component); map_remove(this->map_components_healing_on_change_max_hp, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentDropHeart> component)
    {
        if(component)
        {
            list_remove(this->components_drop_heart, component); map_remove(this->map_components_drop_heart, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentHeart> component)
    {
        if(component)
        {
            list_remove(this->components_heart, component); map_remove(this->map_components_heart, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentHeartAddStats> component)
    {
        if(component)
        {
            list_remove(this->components_heart_add_stats, component); map_remove(this->map_components_heart_add_stats, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentHeartAddStatsHp> component)
    {
        this->remove<ComponentHeartAddStats>(intrusive_ptr<ComponentHeartAddStats>(component));
        if(component)
        {
            list_remove(this->components_heart_add_stats_hp, component); map_remove(this->map_components_heart_add_stats_hp, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentHeartAddStatsDamage> component)
    {
        this->remove<ComponentHeartAddStats>(intrusive_ptr<ComponentHeartAddStats>(component));
        if(component)
        {
            list_remove(this->components_heart_add_stats_damage, component); map_remove(this->map_components_heart_add_stats_damage, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentRandomHealing> component)
    {
        if(component)
        {
            list_remove(this->components_random_healing, component); map_remove(this->map_components_random_healing, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentVampire> component)
    {
        if(component)
        {
            list_remove(this->components_vampire, component); map_remove(this->map_components_vampire, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentHealingLowHp> component)
    {
        if(component)
        {
            list_remove(this->components_healing_low_hp, component); map_remove(this->map_components_healing_low_hp, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentBody> component)
    {
        if(component)
        {
            list_remove(this->components_body, component); map_remove(this->map_components_body, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSwordCast> component)
    {
        if(component)
        {
            list_remove(this->components_sword_cast, component); map_remove(this->map_components_sword_cast, component->id);
        }
    }

    template<> void ModelEcsBase::remove(intrusive_ptr<ComponentSword> component)
    {
        if(component)
        {
            list_remove(this->components_sword, component); map_remove(this->map_components_sword, component->id);
        }
    }

    template<> void ModelEcsBase::remove(ComponentData* component)
    {
        this->remove(intrusive_ptr<ComponentData>(component));
    }
    template<> void ModelEcsBase::remove(Transform* component)
    {
        this->remove(intrusive_ptr<Transform>(component));
    }
    template<> void ModelEcsBase::remove(ComponentStats* component)
    {
        this->remove(intrusive_ptr<ComponentStats>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSide* component)
    {
        this->remove(intrusive_ptr<ComponentSide>(component));
    }
    template<> void ModelEcsBase::remove(ComponentTargetable* component)
    {
        this->remove(intrusive_ptr<ComponentTargetable>(component));
    }
    template<> void ModelEcsBase::remove(ComponentHealth* component)
    {
        this->remove(intrusive_ptr<ComponentHealth>(component));
    }
    template<> void ModelEcsBase::remove(ComponentDamage* component)
    {
        this->remove(intrusive_ptr<ComponentDamage>(component));
    }
    template<> void ModelEcsBase::remove(ComponentUser* component)
    {
        this->remove(intrusive_ptr<ComponentUser>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSpineInfo* component)
    {
        this->remove(intrusive_ptr<ComponentSpineInfo>(component));
    }
    template<> void ModelEcsBase::remove(ComponentBusy* component)
    {
        this->remove(intrusive_ptr<ComponentBusy>(component));
    }
    template<> void ModelEcsBase::remove(ComponentCreateUnitsOnDeath* component)
    {
        this->remove(intrusive_ptr<ComponentCreateUnitsOnDeath>(component));
    }
    template<> void ModelEcsBase::remove(ComponentAura* component)
    {
        this->remove(intrusive_ptr<ComponentAura>(component));
    }
    template<> void ModelEcsBase::remove(ComponentAuraOfSlow* component)
    {
        this->remove(intrusive_ptr<ComponentAuraOfSlow>(component));
    }
    template<> void ModelEcsBase::remove(ComponentAuraDamage* component)
    {
        this->remove(intrusive_ptr<ComponentAuraDamage>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSpirit* component)
    {
        this->remove(intrusive_ptr<ComponentSpirit>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSpiritBasePoints* component)
    {
        this->remove(intrusive_ptr<ComponentSpiritBasePoints>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSpawnSpirit* component)
    {
        this->remove(intrusive_ptr<ComponentSpawnSpirit>(component));
    }
    template<> void ModelEcsBase::remove(ComponentRecharge* component)
    {
        this->remove(intrusive_ptr<ComponentRecharge>(component));
    }
    template<> void ModelEcsBase::remove(ComponentBullet* component)
    {
        this->remove(intrusive_ptr<ComponentBullet>(component));
    }
    template<> void ModelEcsBase::remove(ComponentBulletLaser* component)
    {
        this->remove(intrusive_ptr<ComponentBulletLaser>(component));
    }
    template<> void ModelEcsBase::remove(ComponentShootBullet* component)
    {
        this->remove(intrusive_ptr<ComponentShootBullet>(component));
    }
    template<> void ModelEcsBase::remove(ComponentMeleeAttack* component)
    {
        this->remove(intrusive_ptr<ComponentMeleeAttack>(component));
    }
    template<> void ModelEcsBase::remove(ComponentTargetHighlight* component)
    {
        this->remove(intrusive_ptr<ComponentTargetHighlight>(component));
    }
    template<> void ModelEcsBase::remove(ComponentBulletSplit* component)
    {
        this->remove(intrusive_ptr<ComponentBulletSplit>(component));
    }
    template<> void ModelEcsBase::remove(ComponentShield* component)
    {
        this->remove(intrusive_ptr<ComponentShield>(component));
    }
    template<> void ModelEcsBase::remove(ComponentEffects* component)
    {
        this->remove(intrusive_ptr<ComponentEffects>(component));
    }
    template<> void ModelEcsBase::remove(ComponentBulletFollowToTarget* component)
    {
        this->remove(intrusive_ptr<ComponentBulletFollowToTarget>(component));
    }
    template<> void ModelEcsBase::remove(ComponentMeteorPeriodic* component)
    {
        this->remove(intrusive_ptr<ComponentMeteorPeriodic>(component));
    }
    template<> void ModelEcsBase::remove(ElectricDamage* component)
    {
        this->remove(intrusive_ptr<ElectricDamage>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSphereSpawn* component)
    {
        this->remove(intrusive_ptr<ComponentSphereSpawn>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSphere* component)
    {
        this->remove(intrusive_ptr<ComponentSphere>(component));
    }
    template<> void ModelEcsBase::remove(ComponentActions* component)
    {
        this->remove(intrusive_ptr<ComponentActions>(component));
    }
    template<> void ModelEcsBase::remove(ComponentGate* component)
    {
        this->remove(intrusive_ptr<ComponentGate>(component));
    }
    template<> void ModelEcsBase::remove(ComponentLevelUp* component)
    {
        this->remove(intrusive_ptr<ComponentLevelUp>(component));
    }
    template<> void ModelEcsBase::remove(ComponentExp* component)
    {
        this->remove(intrusive_ptr<ComponentExp>(component));
    }
    template<> void ModelEcsBase::remove(ComponentExpDrop* component)
    {
        this->remove(intrusive_ptr<ComponentExpDrop>(component));
    }
    template<> void ModelEcsBase::remove(ComponentStun* component)
    {
        this->remove(intrusive_ptr<ComponentStun>(component));
    }
    template<> void ModelEcsBase::remove(FireDamage* component)
    {
        this->remove(intrusive_ptr<FireDamage>(component));
    }
    template<> void ModelEcsBase::remove(ComponentMovement* component)
    {
        this->remove(intrusive_ptr<ComponentMovement>(component));
    }
    template<> void ModelEcsBase::remove(MoveDirection* component)
    {
        this->remove(intrusive_ptr<MoveDirection>(component));
    }
    template<> void ModelEcsBase::remove(MoveToTarget* component)
    {
        this->remove(intrusive_ptr<MoveToTarget>(component));
    }
    template<> void ModelEcsBase::remove(MoveInstant* component)
    {
        this->remove(intrusive_ptr<MoveInstant>(component));
    }
    template<> void ModelEcsBase::remove(MoveVertical* component)
    {
        this->remove(intrusive_ptr<MoveVertical>(component));
    }
    template<> void ModelEcsBase::remove(Freezing* component)
    {
        this->remove(intrusive_ptr<Freezing>(component));
    }
    template<> void ModelEcsBase::remove(MoveParabolic* component)
    {
        this->remove(intrusive_ptr<MoveParabolic>(component));
    }
    template<> void ModelEcsBase::remove(ComponentCreateMovementToHero* component)
    {
        this->remove(intrusive_ptr<ComponentCreateMovementToHero>(component));
    }
    template<> void ModelEcsBase::remove(ComponentPush* component)
    {
        this->remove(intrusive_ptr<ComponentPush>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSpawn* component)
    {
        this->remove(intrusive_ptr<ComponentSpawn>(component));
    }
    template<> void ModelEcsBase::remove(ComponentHealingOnChangeMaxHp* component)
    {
        this->remove(intrusive_ptr<ComponentHealingOnChangeMaxHp>(component));
    }
    template<> void ModelEcsBase::remove(ComponentDropHeart* component)
    {
        this->remove(intrusive_ptr<ComponentDropHeart>(component));
    }
    template<> void ModelEcsBase::remove(ComponentHeart* component)
    {
        this->remove(intrusive_ptr<ComponentHeart>(component));
    }
    template<> void ModelEcsBase::remove(ComponentHeartAddStats* component)
    {
        this->remove(intrusive_ptr<ComponentHeartAddStats>(component));
    }
    template<> void ModelEcsBase::remove(ComponentHeartAddStatsHp* component)
    {
        this->remove(intrusive_ptr<ComponentHeartAddStatsHp>(component));
    }
    template<> void ModelEcsBase::remove(ComponentHeartAddStatsDamage* component)
    {
        this->remove(intrusive_ptr<ComponentHeartAddStatsDamage>(component));
    }
    template<> void ModelEcsBase::remove(ComponentRandomHealing* component)
    {
        this->remove(intrusive_ptr<ComponentRandomHealing>(component));
    }
    template<> void ModelEcsBase::remove(ComponentVampire* component)
    {
        this->remove(intrusive_ptr<ComponentVampire>(component));
    }
    template<> void ModelEcsBase::remove(ComponentHealingLowHp* component)
    {
        this->remove(intrusive_ptr<ComponentHealingLowHp>(component));
    }
    template<> void ModelEcsBase::remove(ComponentBody* component)
    {
        this->remove(intrusive_ptr<ComponentBody>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSwordCast* component)
    {
        this->remove(intrusive_ptr<ComponentSwordCast>(component));
    }
    template<> void ModelEcsBase::remove(ComponentSword* component)
    {
        this->remove(intrusive_ptr<ComponentSword>(component));
    }
    int ModelEcsBase::get_free_id()
    {
        auto id = this->next_free_id;
        this->next_free_id += 1;
        return id;
    }

    void ModelEcsBase::subscribe()
    {

    }

    void ModelEcsBase::unsubscribe()
    {

    }

    void ModelEcsBase::update(float dt)
    {
        this->game_timer += dt;
        this->update_systems(dt);
    }

    void ModelEcsBase::update_systems(float dt)
    {
        ;
        SystemShieldRestore s2;
        s2.update(this, dt);
        SystemRemoveDamage s3;
        s3.update(this, dt);
        s3.clean(this);
        ;
        SystemSpawn s6;
        s6.update(this, dt);
        SystemGate s7;
        s7.update(this, dt);
        SystemSpiritSpawn s8;
        s8.update(this, dt);
        ;
        SystemActions s11;
        s11.update(this, dt);
        SystemMeteor s12;
        s12.update(this, dt);
        ;
        SystemHealingOnChangeMaxHp s15;
        s15.update(this, dt);
        ;
        SystemAuraDamage s18;
        s18.update(this, dt);
        SystemShoot s19;
        s19.update(this, dt);
        SystemSwordsCast s20;
        s20.update(this, dt);
        s20.clean(this);
        SystemSwords s21;
        s21.update(this, dt);
        SystemMeleeAttack s22;
        s22.update(this, dt);
        SystemBullet s23;
        s23.update(this, dt);
        SystemFireDamage s24;
        s24.update(this, dt);
        SystemDamage s25;
        s25.update(this, dt);
        SystemCreateUnitsOnDeath s26;
        s26.update(this, dt);
        SystemExp s27;
        s27.update(this, dt);
        s27.clean(this);
        SystemDropHeart s28;
        s28.update(this, dt);
        s28.clean(this);
        SystemDeath s29;
        s29.update(this, dt);
        s29.clean(this);
        SystemBulletFollowToTarget s30;
        s30.update(this, dt);
        SystemEffects s31;
        s31.update(this, dt);
        SystemBulletClean s32;
        s32.update(this, dt);
        s32.clean(this);
        ;
        SystemFreezing s35;
        s35.update(this, dt);
        SystemStun s36;
        s36.update(this, dt);
        SystemAuroOfSlow s37;
        s37.update(this, dt);
        SystemCreateMovement s38;
        s38.update(this, dt);
        SystemMovement s39;
        s39.update(this, dt);
        s39.clean(this);
        SystemPush s40;
        s40.update(this, dt);
        s40.clean(this);
        SystemResolveCollisions s41;
        s41.update(this, dt);
        SystemSpiritMovement s42;
        s42.update(this, dt);
        SystemSphere s43;
        s43.update(this, dt);
        ;
    }

    void ModelEcsBase::add_entity(int id)
    {
        list_push(this->entities, id);
    }

    void ModelEcsBase::remove_entity(int id)
    {
        if(in_list(id, this->entities))
        {
            list_remove(this->entities, id);
        }
        this->event_remove_entity.notify(id);

        if(in_map(id, this->map_components_data))
        {
            auto component = this->map_components_data.at(id);
            this->remove<ComponentData>(component);
        }
        if(in_map(id, this->map_components_transform))
        {
            auto component = this->map_components_transform.at(id);
            this->remove<Transform>(component);
        }
        if(in_map(id, this->map_components_stats))
        {
            auto component = this->map_components_stats.at(id);
            this->remove<ComponentStats>(component);
        }
        if(in_map(id, this->map_components_side))
        {
            auto component = this->map_components_side.at(id);
            this->remove<ComponentSide>(component);
        }
        if(in_map(id, this->map_components_targetable))
        {
            auto component = this->map_components_targetable.at(id);
            this->remove<ComponentTargetable>(component);
        }
        if(in_map(id, this->map_components_health))
        {
            auto component = this->map_components_health.at(id);
            this->remove<ComponentHealth>(component);
        }
        if(in_map(id, this->map_components_damage))
        {
            auto component = this->map_components_damage.at(id);
            this->remove<ComponentDamage>(component);
        }
        if(in_map(id, this->map_components_user))
        {
            auto component = this->map_components_user.at(id);
            this->remove<ComponentUser>(component);
        }
        if(in_map(id, this->map_components_spine_info))
        {
            auto component = this->map_components_spine_info.at(id);
            this->remove<ComponentSpineInfo>(component);
        }
        if(in_map(id, this->map_components_busy))
        {
            auto component = this->map_components_busy.at(id);
            this->remove<ComponentBusy>(component);
        }
        if(in_map(id, this->map_components_create_units_on_death))
        {
            auto component = this->map_components_create_units_on_death.at(id);
            this->remove<ComponentCreateUnitsOnDeath>(component);
        }
        if(in_map(id, this->map_components_aura))
        {
            auto component = this->map_components_aura.at(id);
            this->remove<ComponentAura>(component);
        }
        if(in_map(id, this->map_components_aura_of_slow))
        {
            auto component = this->map_components_aura_of_slow.at(id);
            this->remove<ComponentAuraOfSlow>(component);
        }
        if(in_map(id, this->map_components_aura_damage))
        {
            auto component = this->map_components_aura_damage.at(id);
            this->remove<ComponentAuraDamage>(component);
        }
        if(in_map(id, this->map_components_spirit))
        {
            auto component = this->map_components_spirit.at(id);
            this->remove<ComponentSpirit>(component);
        }
        if(in_map(id, this->map_components_spirit_base_points))
        {
            auto component = this->map_components_spirit_base_points.at(id);
            this->remove<ComponentSpiritBasePoints>(component);
        }
        if(in_map(id, this->map_components_spawn_spirit))
        {
            auto component = this->map_components_spawn_spirit.at(id);
            this->remove<ComponentSpawnSpirit>(component);
        }
        if(in_map(id, this->map_components_recharge))
        {
            auto component = this->map_components_recharge.at(id);
            this->remove<ComponentRecharge>(component);
        }
        if(in_map(id, this->map_components_bullet))
        {
            auto component = this->map_components_bullet.at(id);
            this->remove<ComponentBullet>(component);
        }
        if(in_map(id, this->map_components_bullet_laser))
        {
            auto component = this->map_components_bullet_laser.at(id);
            this->remove<ComponentBulletLaser>(component);
        }
        if(in_map(id, this->map_components_shoot_bullet))
        {
            auto component = this->map_components_shoot_bullet.at(id);
            this->remove<ComponentShootBullet>(component);
        }
        if(in_map(id, this->map_components_melee_attack))
        {
            auto component = this->map_components_melee_attack.at(id);
            this->remove<ComponentMeleeAttack>(component);
        }
        if(in_map(id, this->map_components_target_highlight))
        {
            auto component = this->map_components_target_highlight.at(id);
            this->remove<ComponentTargetHighlight>(component);
        }
        if(in_map(id, this->map_components_bullet_split))
        {
            auto component = this->map_components_bullet_split.at(id);
            this->remove<ComponentBulletSplit>(component);
        }
        if(in_map(id, this->map_components_shield))
        {
            auto component = this->map_components_shield.at(id);
            this->remove<ComponentShield>(component);
        }
        if(in_map(id, this->map_components_effects))
        {
            auto component = this->map_components_effects.at(id);
            this->remove<ComponentEffects>(component);
        }
        if(in_map(id, this->map_components_bullet_follow_to_target))
        {
            auto component = this->map_components_bullet_follow_to_target.at(id);
            this->remove<ComponentBulletFollowToTarget>(component);
        }
        if(in_map(id, this->map_components_meteor_periodic))
        {
            auto component = this->map_components_meteor_periodic.at(id);
            this->remove<ComponentMeteorPeriodic>(component);
        }
        if(in_map(id, this->map_components_electric_damage))
        {
            auto component = this->map_components_electric_damage.at(id);
            this->remove<ElectricDamage>(component);
        }
        if(in_map(id, this->map_components_sphere_spawn))
        {
            auto component = this->map_components_sphere_spawn.at(id);
            this->remove<ComponentSphereSpawn>(component);
        }
        if(in_map(id, this->map_components_sphere))
        {
            auto component = this->map_components_sphere.at(id);
            this->remove<ComponentSphere>(component);
        }
        if(in_map(id, this->map_components_actions))
        {
            auto component = this->map_components_actions.at(id);
            this->remove<ComponentActions>(component);
        }
        if(in_map(id, this->map_components_gate))
        {
            auto component = this->map_components_gate.at(id);
            this->remove<ComponentGate>(component);
        }
        if(in_map(id, this->map_components_level_up))
        {
            auto component = this->map_components_level_up.at(id);
            this->remove<ComponentLevelUp>(component);
        }
        if(in_map(id, this->map_components_exp))
        {
            auto component = this->map_components_exp.at(id);
            this->remove<ComponentExp>(component);
        }
        if(in_map(id, this->map_components_exp_drop))
        {
            auto component = this->map_components_exp_drop.at(id);
            this->remove<ComponentExpDrop>(component);
        }
        if(in_map(id, this->map_components_stun))
        {
            auto component = this->map_components_stun.at(id);
            this->remove<ComponentStun>(component);
        }
        if(in_map(id, this->map_components_fire_damage))
        {
            auto component = this->map_components_fire_damage.at(id);
            this->remove<FireDamage>(component);
        }
        if(in_map(id, this->map_components_movement))
        {
            auto component = this->map_components_movement.at(id);
            this->remove<ComponentMovement>(component);
        }
        if(in_map(id, this->map_components_move_direction))
        {
            auto component = this->map_components_move_direction.at(id);
            this->remove<MoveDirection>(component);
        }
        if(in_map(id, this->map_components_move_to_target))
        {
            auto component = this->map_components_move_to_target.at(id);
            this->remove<MoveToTarget>(component);
        }
        if(in_map(id, this->map_components_move_instant))
        {
            auto component = this->map_components_move_instant.at(id);
            this->remove<MoveInstant>(component);
        }
        if(in_map(id, this->map_components_move_vertical))
        {
            auto component = this->map_components_move_vertical.at(id);
            this->remove<MoveVertical>(component);
        }
        if(in_map(id, this->map_components_freezing))
        {
            auto component = this->map_components_freezing.at(id);
            this->remove<Freezing>(component);
        }
        if(in_map(id, this->map_components_move_parabolic))
        {
            auto component = this->map_components_move_parabolic.at(id);
            this->remove<MoveParabolic>(component);
        }
        if(in_map(id, this->map_components_create_movement_to_hero))
        {
            auto component = this->map_components_create_movement_to_hero.at(id);
            this->remove<ComponentCreateMovementToHero>(component);
        }
        if(in_map(id, this->map_components_push))
        {
            auto component = this->map_components_push.at(id);
            this->remove<ComponentPush>(component);
        }
        if(in_map(id, this->map_components_spawn))
        {
            auto component = this->map_components_spawn.at(id);
            this->remove<ComponentSpawn>(component);
        }
        if(in_map(id, this->map_components_healing_on_change_max_hp))
        {
            auto component = this->map_components_healing_on_change_max_hp.at(id);
            this->remove<ComponentHealingOnChangeMaxHp>(component);
        }
        if(in_map(id, this->map_components_drop_heart))
        {
            auto component = this->map_components_drop_heart.at(id);
            this->remove<ComponentDropHeart>(component);
        }
        if(in_map(id, this->map_components_heart))
        {
            auto component = this->map_components_heart.at(id);
            this->remove<ComponentHeart>(component);
        }
        if(in_map(id, this->map_components_heart_add_stats))
        {
            auto component = this->map_components_heart_add_stats.at(id);
            this->remove<ComponentHeartAddStats>(component);
        }
        if(in_map(id, this->map_components_heart_add_stats_hp))
        {
            auto component = this->map_components_heart_add_stats_hp.at(id);
            this->remove<ComponentHeartAddStatsHp>(component);
        }
        if(in_map(id, this->map_components_heart_add_stats_damage))
        {
            auto component = this->map_components_heart_add_stats_damage.at(id);
            this->remove<ComponentHeartAddStatsDamage>(component);
        }
        if(in_map(id, this->map_components_random_healing))
        {
            auto component = this->map_components_random_healing.at(id);
            this->remove<ComponentRandomHealing>(component);
        }
        if(in_map(id, this->map_components_vampire))
        {
            auto component = this->map_components_vampire.at(id);
            this->remove<ComponentVampire>(component);
        }
        if(in_map(id, this->map_components_healing_low_hp))
        {
            auto component = this->map_components_healing_low_hp.at(id);
            this->remove<ComponentHealingLowHp>(component);
        }
        if(in_map(id, this->map_components_body))
        {
            auto component = this->map_components_body.at(id);
            this->remove<ComponentBody>(component);
        }
        if(in_map(id, this->map_components_sword_cast))
        {
            auto component = this->map_components_sword_cast.at(id);
            this->remove<ComponentSwordCast>(component);
        }
        if(in_map(id, this->map_components_sword))
        {
            auto component = this->map_components_sword.at(id);
            this->remove<ComponentSword>(component);
        }
    }

    void ModelEcsBase::on_loaded()
    {

    }

    int ModelEcsBase::get_entity_with_name(const std::string& name) const
    {
        return 0;
    }

    float ModelEcsBase::get_health_rate() const
    {
        return 1.0f;
    }

    void ModelEcsBase::retain()
    {
        ++this->_reference_counter;
    }

    int ModelEcsBase::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool ModelEcsBase::operator ==(const ModelEcsBase& rhs) const
    {
        bool result = true;
        result = result && ((this->user == rhs.user) || (this->user != nullptr && rhs.user != nullptr && *this->user == *rhs.user));
        result = result && this->next_free_id == rhs.next_free_id;
        result = result && this->player_id == rhs.player_id;
        result = result && this->entities == rhs.entities;
        result = result && this->game_timer == rhs.game_timer;
        result = result && this->tasks == rhs.tasks;
        result = result && ((this->data == rhs.data) || (this->data != nullptr && rhs.data != nullptr && *this->data == *rhs.data));
        result = result && this->wave_index == rhs.wave_index;
        result = result && ((this->ground == rhs.ground) || (this->ground != nullptr && rhs.ground != nullptr && *this->ground == *rhs.ground));
        result = result && this->enemies_level == rhs.enemies_level;
        result = result && this->enemies_rank == rhs.enemies_rank;
        result = result && this->wave_finished == rhs.wave_finished;
        result = result && this->timer_wave_interval == rhs.timer_wave_interval;
        result = result && this->timer_wave_duration == rhs.timer_wave_duration;
        result = result && this->has_skills == rhs.has_skills;
        result = result && this->spawn_points == rhs.spawn_points;
        return result;
    }

    bool ModelEcsBase::operator !=(const ModelEcsBase& rhs) const
    {
        return !(*this == rhs);
    }

    ModelEcsBase::ModelEcsBase(const ModelEcsBase& rhs)
    {
        this->operator=(rhs);
    }

    const ModelEcsBase& ModelEcsBase::operator =(const ModelEcsBase& rhs)
    {
        this->user = rhs.user;
        this->next_free_id = rhs.next_free_id;
        this->player_id = rhs.player_id;
        this->entities = rhs.entities;
        this->game_timer = rhs.game_timer;
        this->tasks = rhs.tasks;
        this->data = rhs.data;
        this->wave_index = rhs.wave_index;
        this->ground = rhs.ground;
        this->enemies_level = rhs.enemies_level;
        this->enemies_rank = rhs.enemies_rank;
        this->wave_finished = rhs.wave_finished;
        this->timer_wave_interval = rhs.timer_wave_interval;
        this->timer_wave_duration = rhs.timer_wave_duration;
        this->has_skills = rhs.has_skills;
        this->event_did_initialized = rhs.event_did_initialized;
        this->event_wave_finish = rhs.event_wave_finish;
        this->event_wave_start = rhs.event_wave_start;
        this->event_wave_next = rhs.event_wave_next;
        this->event_exit = rhs.event_exit;
        this->event_create_entity = rhs.event_create_entity;
        this->event_create_bullet = rhs.event_create_bullet;
        this->event_remove_entity = rhs.event_remove_entity;
        this->event_create_marker = rhs.event_create_marker;
        this->event_hero_level_up = rhs.event_hero_level_up;
        this->event_hero_exp_changed = rhs.event_hero_exp_changed;
        this->event_prepare_to_shoot = rhs.event_prepare_to_shoot;
        this->event_turn_to = rhs.event_turn_to;
        this->event_shoot = rhs.event_shoot;
        this->event_death = rhs.event_death;
        this->event_skill_animate = rhs.event_skill_animate;
        this->event_healing = rhs.event_healing;
        this->event_shield_activated = rhs.event_shield_activated;
        this->event_dodge = rhs.event_dodge;
        this->event_ressurection = rhs.event_ressurection;
        this->event_effects = rhs.event_effects;
        this->event_freeze_activated = rhs.event_freeze_activated;
        this->event_freeze_deactivated = rhs.event_freeze_deactivated;
        this->event_fire_activated = rhs.event_fire_activated;
        this->event_fire_deactivated = rhs.event_fire_deactivated;
        this->event_stun_activated = rhs.event_stun_activated;
        this->event_stun_deactivated = rhs.event_stun_deactivated;
        this->event_damaged = rhs.event_damaged;
        this->event_aura_activated = rhs.event_aura_activated;
        this->spawn_points = rhs.spawn_points;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string ModelEcsBase::get_type() const
    {
        return ModelEcsBase::TYPE;
    }

    void ModelEcsBase::clear_components_data()
    {
        map_clear(this->map_components_data);
        list_clear(this->components_data);
    }

    void ModelEcsBase::clear_components_transform()
    {
        map_clear(this->map_components_transform);
        list_clear(this->components_transform);
    }

    void ModelEcsBase::clear_components_stats()
    {
        map_clear(this->map_components_stats);
        list_clear(this->components_stats);
    }

    void ModelEcsBase::clear_components_side()
    {
        map_clear(this->map_components_side);
        list_clear(this->components_side);
    }

    void ModelEcsBase::clear_components_targetable()
    {
        map_clear(this->map_components_targetable);
        list_clear(this->components_targetable);
    }

    void ModelEcsBase::clear_components_health()
    {
        map_clear(this->map_components_health);
        list_clear(this->components_health);
    }

    void ModelEcsBase::clear_components_damage()
    {
        map_clear(this->map_components_damage);
        list_clear(this->components_damage);
    }

    void ModelEcsBase::clear_components_user()
    {
        map_clear(this->map_components_user);
        list_clear(this->components_user);
    }

    void ModelEcsBase::clear_components_spine_info()
    {
        map_clear(this->map_components_spine_info);
        list_clear(this->components_spine_info);
    }

    void ModelEcsBase::clear_components_busy()
    {
        map_clear(this->map_components_busy);
        list_clear(this->components_busy);
    }

    void ModelEcsBase::clear_components_create_units_on_death()
    {
        map_clear(this->map_components_create_units_on_death);
        list_clear(this->components_create_units_on_death);
    }

    void ModelEcsBase::clear_components_aura()
    {
        map_clear(this->map_components_aura);
        list_clear(this->components_aura);
    }

    void ModelEcsBase::clear_components_aura_of_slow()
    {
        map_clear(this->map_components_aura_of_slow);
        list_clear(this->components_aura_of_slow);
    }

    void ModelEcsBase::clear_components_aura_damage()
    {
        map_clear(this->map_components_aura_damage);
        list_clear(this->components_aura_damage);
    }

    void ModelEcsBase::clear_components_spirit()
    {
        map_clear(this->map_components_spirit);
        list_clear(this->components_spirit);
    }

    void ModelEcsBase::clear_components_spirit_base_points()
    {
        map_clear(this->map_components_spirit_base_points);
        list_clear(this->components_spirit_base_points);
    }

    void ModelEcsBase::clear_components_spawn_spirit()
    {
        map_clear(this->map_components_spawn_spirit);
        list_clear(this->components_spawn_spirit);
    }

    void ModelEcsBase::clear_components_recharge()
    {
        map_clear(this->map_components_recharge);
        list_clear(this->components_recharge);
    }

    void ModelEcsBase::clear_components_bullet()
    {
        map_clear(this->map_components_bullet);
        list_clear(this->components_bullet);
    }

    void ModelEcsBase::clear_components_bullet_laser()
    {
        map_clear(this->map_components_bullet_laser);
        list_clear(this->components_bullet_laser);
    }

    void ModelEcsBase::clear_components_shoot_bullet()
    {
        map_clear(this->map_components_shoot_bullet);
        list_clear(this->components_shoot_bullet);
    }

    void ModelEcsBase::clear_components_melee_attack()
    {
        map_clear(this->map_components_melee_attack);
        list_clear(this->components_melee_attack);
    }

    void ModelEcsBase::clear_components_target_highlight()
    {
        map_clear(this->map_components_target_highlight);
        list_clear(this->components_target_highlight);
    }

    void ModelEcsBase::clear_components_bullet_split()
    {
        map_clear(this->map_components_bullet_split);
        list_clear(this->components_bullet_split);
    }

    void ModelEcsBase::clear_components_shield()
    {
        map_clear(this->map_components_shield);
        list_clear(this->components_shield);
    }

    void ModelEcsBase::clear_components_effects()
    {
        map_clear(this->map_components_effects);
        list_clear(this->components_effects);
    }

    void ModelEcsBase::clear_components_bullet_follow_to_target()
    {
        map_clear(this->map_components_bullet_follow_to_target);
        list_clear(this->components_bullet_follow_to_target);
    }

    void ModelEcsBase::clear_components_meteor_periodic()
    {
        map_clear(this->map_components_meteor_periodic);
        list_clear(this->components_meteor_periodic);
    }

    void ModelEcsBase::clear_components_electric_damage()
    {
        map_clear(this->map_components_electric_damage);
        list_clear(this->components_electric_damage);
    }

    void ModelEcsBase::clear_components_sphere_spawn()
    {
        map_clear(this->map_components_sphere_spawn);
        list_clear(this->components_sphere_spawn);
    }

    void ModelEcsBase::clear_components_sphere()
    {
        map_clear(this->map_components_sphere);
        list_clear(this->components_sphere);
    }

    void ModelEcsBase::clear_components_actions()
    {
        map_clear(this->map_components_actions);
        list_clear(this->components_actions);
    }

    void ModelEcsBase::clear_components_gate()
    {
        map_clear(this->map_components_gate);
        list_clear(this->components_gate);
    }

    void ModelEcsBase::clear_components_level_up()
    {
        map_clear(this->map_components_level_up);
        list_clear(this->components_level_up);
    }

    void ModelEcsBase::clear_components_exp()
    {
        map_clear(this->map_components_exp);
        list_clear(this->components_exp);
    }

    void ModelEcsBase::clear_components_exp_drop()
    {
        map_clear(this->map_components_exp_drop);
        list_clear(this->components_exp_drop);
    }

    void ModelEcsBase::clear_components_stun()
    {
        map_clear(this->map_components_stun);
        list_clear(this->components_stun);
    }

    void ModelEcsBase::clear_components_fire_damage()
    {
        map_clear(this->map_components_fire_damage);
        list_clear(this->components_fire_damage);
    }

    void ModelEcsBase::clear_components_movement()
    {
        map_clear(this->map_components_movement);
        list_clear(this->components_movement);
    }

    void ModelEcsBase::clear_components_move_direction()
    {
        map_clear(this->map_components_move_direction);
        list_clear(this->components_move_direction);
    }

    void ModelEcsBase::clear_components_move_to_target()
    {
        map_clear(this->map_components_move_to_target);
        list_clear(this->components_move_to_target);
    }

    void ModelEcsBase::clear_components_move_instant()
    {
        map_clear(this->map_components_move_instant);
        list_clear(this->components_move_instant);
    }

    void ModelEcsBase::clear_components_move_vertical()
    {
        map_clear(this->map_components_move_vertical);
        list_clear(this->components_move_vertical);
    }

    void ModelEcsBase::clear_components_freezing()
    {
        map_clear(this->map_components_freezing);
        list_clear(this->components_freezing);
    }

    void ModelEcsBase::clear_components_move_parabolic()
    {
        map_clear(this->map_components_move_parabolic);
        list_clear(this->components_move_parabolic);
    }

    void ModelEcsBase::clear_components_create_movement_to_hero()
    {
        map_clear(this->map_components_create_movement_to_hero);
        list_clear(this->components_create_movement_to_hero);
    }

    void ModelEcsBase::clear_components_push()
    {
        map_clear(this->map_components_push);
        list_clear(this->components_push);
    }

    void ModelEcsBase::clear_components_spawn()
    {
        map_clear(this->map_components_spawn);
        list_clear(this->components_spawn);
    }

    void ModelEcsBase::clear_components_healing_on_change_max_hp()
    {
        map_clear(this->map_components_healing_on_change_max_hp);
        list_clear(this->components_healing_on_change_max_hp);
    }

    void ModelEcsBase::clear_components_drop_heart()
    {
        map_clear(this->map_components_drop_heart);
        list_clear(this->components_drop_heart);
    }

    void ModelEcsBase::clear_components_heart()
    {
        map_clear(this->map_components_heart);
        list_clear(this->components_heart);
    }

    void ModelEcsBase::clear_components_heart_add_stats()
    {
        map_clear(this->map_components_heart_add_stats);
        list_clear(this->components_heart_add_stats);
    }

    void ModelEcsBase::clear_components_heart_add_stats_hp()
    {
        map_clear(this->map_components_heart_add_stats_hp);
        list_clear(this->components_heart_add_stats_hp);
    }

    void ModelEcsBase::clear_components_heart_add_stats_damage()
    {
        map_clear(this->map_components_heart_add_stats_damage);
        list_clear(this->components_heart_add_stats_damage);
    }

    void ModelEcsBase::clear_components_random_healing()
    {
        map_clear(this->map_components_random_healing);
        list_clear(this->components_random_healing);
    }

    void ModelEcsBase::clear_components_vampire()
    {
        map_clear(this->map_components_vampire);
        list_clear(this->components_vampire);
    }

    void ModelEcsBase::clear_components_healing_low_hp()
    {
        map_clear(this->map_components_healing_low_hp);
        list_clear(this->components_healing_low_hp);
    }

    void ModelEcsBase::clear_components_body()
    {
        map_clear(this->map_components_body);
        list_clear(this->components_body);
    }

    void ModelEcsBase::clear_components_sword_cast()
    {
        map_clear(this->map_components_sword_cast);
        list_clear(this->components_sword_cast);
    }

    void ModelEcsBase::clear_components_sword()
    {
        map_clear(this->map_components_sword);
        list_clear(this->components_sword);
    }

    void ModelEcsBase::build_maps()
    {
        for(auto& component : this->components_data)
        {
            this->map_components_data[component->id] = component;
        }
        for(auto& component : this->components_transform)
        {
            this->map_components_transform[component->id] = component;
        }
        for(auto& component : this->components_stats)
        {
            this->map_components_stats[component->id] = component;
        }
        for(auto& component : this->components_side)
        {
            this->map_components_side[component->id] = component;
        }
        for(auto& component : this->components_targetable)
        {
            this->map_components_targetable[component->id] = component;
        }
        for(auto& component : this->components_health)
        {
            this->map_components_health[component->id] = component;
        }
        for(auto& component : this->components_damage)
        {
            this->map_components_damage[component->id] = component;
        }
        for(auto& component : this->components_user)
        {
            this->map_components_user[component->id] = component;
        }
        for(auto& component : this->components_spine_info)
        {
            this->map_components_spine_info[component->id] = component;
        }
        for(auto& component : this->components_busy)
        {
            this->map_components_busy[component->id] = component;
        }
        for(auto& component : this->components_create_units_on_death)
        {
            this->map_components_create_units_on_death[component->id] = component;
        }
        for(auto& component : this->components_aura)
        {
            this->map_components_aura[component->id] = component;
        }
        for(auto& component : this->components_aura_of_slow)
        {
            this->map_components_aura_of_slow[component->id] = component;
        }
        for(auto& component : this->components_aura_damage)
        {
            this->map_components_aura_damage[component->id] = component;
        }
        for(auto& component : this->components_spirit)
        {
            this->map_components_spirit[component->id] = component;
        }
        for(auto& component : this->components_spirit_base_points)
        {
            this->map_components_spirit_base_points[component->id] = component;
        }
        for(auto& component : this->components_spawn_spirit)
        {
            this->map_components_spawn_spirit[component->id] = component;
        }
        for(auto& component : this->components_recharge)
        {
            this->map_components_recharge[component->id] = component;
        }
        for(auto& component : this->components_bullet)
        {
            this->map_components_bullet[component->id] = component;
        }
        for(auto& component : this->components_bullet_laser)
        {
            this->map_components_bullet_laser[component->id] = component;
        }
        for(auto& component : this->components_shoot_bullet)
        {
            this->map_components_shoot_bullet[component->id] = component;
        }
        for(auto& component : this->components_melee_attack)
        {
            this->map_components_melee_attack[component->id] = component;
        }
        for(auto& component : this->components_target_highlight)
        {
            this->map_components_target_highlight[component->id] = component;
        }
        for(auto& component : this->components_bullet_split)
        {
            this->map_components_bullet_split[component->id] = component;
        }
        for(auto& component : this->components_shield)
        {
            this->map_components_shield[component->id] = component;
        }
        for(auto& component : this->components_effects)
        {
            this->map_components_effects[component->id] = component;
        }
        for(auto& component : this->components_bullet_follow_to_target)
        {
            this->map_components_bullet_follow_to_target[component->id] = component;
        }
        for(auto& component : this->components_meteor_periodic)
        {
            this->map_components_meteor_periodic[component->id] = component;
        }
        for(auto& component : this->components_electric_damage)
        {
            this->map_components_electric_damage[component->id] = component;
        }
        for(auto& component : this->components_sphere_spawn)
        {
            this->map_components_sphere_spawn[component->id] = component;
        }
        for(auto& component : this->components_sphere)
        {
            this->map_components_sphere[component->id] = component;
        }
        for(auto& component : this->components_actions)
        {
            this->map_components_actions[component->id] = component;
        }
        for(auto& component : this->components_gate)
        {
            this->map_components_gate[component->id] = component;
        }
        for(auto& component : this->components_level_up)
        {
            this->map_components_level_up[component->id] = component;
        }
        for(auto& component : this->components_exp)
        {
            this->map_components_exp[component->id] = component;
        }
        for(auto& component : this->components_exp_drop)
        {
            this->map_components_exp_drop[component->id] = component;
        }
        for(auto& component : this->components_stun)
        {
            this->map_components_stun[component->id] = component;
        }
        for(auto& component : this->components_fire_damage)
        {
            this->map_components_fire_damage[component->id] = component;
        }
        for(auto& component : this->components_movement)
        {
            this->map_components_movement[component->id] = component;
        }
        for(auto& component : this->components_move_direction)
        {
            this->map_components_move_direction[component->id] = component;
        }
        for(auto& component : this->components_move_to_target)
        {
            this->map_components_move_to_target[component->id] = component;
        }
        for(auto& component : this->components_move_instant)
        {
            this->map_components_move_instant[component->id] = component;
        }
        for(auto& component : this->components_move_vertical)
        {
            this->map_components_move_vertical[component->id] = component;
        }
        for(auto& component : this->components_freezing)
        {
            this->map_components_freezing[component->id] = component;
        }
        for(auto& component : this->components_move_parabolic)
        {
            this->map_components_move_parabolic[component->id] = component;
        }
        for(auto& component : this->components_create_movement_to_hero)
        {
            this->map_components_create_movement_to_hero[component->id] = component;
        }
        for(auto& component : this->components_push)
        {
            this->map_components_push[component->id] = component;
        }
        for(auto& component : this->components_spawn)
        {
            this->map_components_spawn[component->id] = component;
        }
        for(auto& component : this->components_healing_on_change_max_hp)
        {
            this->map_components_healing_on_change_max_hp[component->id] = component;
        }
        for(auto& component : this->components_drop_heart)
        {
            this->map_components_drop_heart[component->id] = component;
        }
        for(auto& component : this->components_heart)
        {
            this->map_components_heart[component->id] = component;
        }
        for(auto& component : this->components_heart_add_stats)
        {
            this->map_components_heart_add_stats[component->id] = component;
        }
        for(auto& component : this->components_heart_add_stats_hp)
        {
            this->map_components_heart_add_stats_hp[component->id] = component;
        }
        for(auto& component : this->components_heart_add_stats_damage)
        {
            this->map_components_heart_add_stats_damage[component->id] = component;
        }
        for(auto& component : this->components_random_healing)
        {
            this->map_components_random_healing[component->id] = component;
        }
        for(auto& component : this->components_vampire)
        {
            this->map_components_vampire[component->id] = component;
        }
        for(auto& component : this->components_healing_low_hp)
        {
            this->map_components_healing_low_hp[component->id] = component;
        }
        for(auto& component : this->components_body)
        {
            this->map_components_body[component->id] = component;
        }
        for(auto& component : this->components_sword_cast)
        {
            this->map_components_sword_cast[component->id] = component;
        }
        for(auto& component : this->components_sword)
        {
            this->map_components_sword[component->id] = component;
        }
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentData> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_data))
        {
            list_remove(this->components_data, this->map_components_data.at(component->id));
        }
        auto iter = std::lower_bound(this->components_data.begin(), this->components_data.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_data.insert(iter, component);
        assert(std::is_sorted(this->components_data.begin(), this->components_data.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_data[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<Transform> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_transform))
        {
            list_remove(this->components_transform, this->map_components_transform.at(component->id));
        }
        auto iter = std::lower_bound(this->components_transform.begin(), this->components_transform.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_transform.insert(iter, component);
        assert(std::is_sorted(this->components_transform.begin(), this->components_transform.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_transform[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentStats> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_stats))
        {
            list_remove(this->components_stats, this->map_components_stats.at(component->id));
        }
        auto iter = std::lower_bound(this->components_stats.begin(), this->components_stats.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_stats.insert(iter, component);
        assert(std::is_sorted(this->components_stats.begin(), this->components_stats.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_stats[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSide> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_side))
        {
            list_remove(this->components_side, this->map_components_side.at(component->id));
        }
        auto iter = std::lower_bound(this->components_side.begin(), this->components_side.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_side.insert(iter, component);
        assert(std::is_sorted(this->components_side.begin(), this->components_side.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_side[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentTargetable> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_targetable))
        {
            list_remove(this->components_targetable, this->map_components_targetable.at(component->id));
        }
        auto iter = std::lower_bound(this->components_targetable.begin(), this->components_targetable.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_targetable.insert(iter, component);
        assert(std::is_sorted(this->components_targetable.begin(), this->components_targetable.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_targetable[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentHealth> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_health))
        {
            list_remove(this->components_health, this->map_components_health.at(component->id));
        }
        auto iter = std::lower_bound(this->components_health.begin(), this->components_health.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_health.insert(iter, component);
        assert(std::is_sorted(this->components_health.begin(), this->components_health.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_health[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentDamage> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_damage))
        {
            list_remove(this->components_damage, this->map_components_damage.at(component->id));
        }
        auto iter = std::lower_bound(this->components_damage.begin(), this->components_damage.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_damage.insert(iter, component);
        assert(std::is_sorted(this->components_damage.begin(), this->components_damage.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_damage[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentUser> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_user))
        {
            list_remove(this->components_user, this->map_components_user.at(component->id));
        }
        auto iter = std::lower_bound(this->components_user.begin(), this->components_user.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_user.insert(iter, component);
        assert(std::is_sorted(this->components_user.begin(), this->components_user.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_user[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSpineInfo> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_spine_info))
        {
            list_remove(this->components_spine_info, this->map_components_spine_info.at(component->id));
        }
        auto iter = std::lower_bound(this->components_spine_info.begin(), this->components_spine_info.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_spine_info.insert(iter, component);
        assert(std::is_sorted(this->components_spine_info.begin(), this->components_spine_info.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_spine_info[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentBusy> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_busy))
        {
            list_remove(this->components_busy, this->map_components_busy.at(component->id));
        }
        auto iter = std::lower_bound(this->components_busy.begin(), this->components_busy.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_busy.insert(iter, component);
        assert(std::is_sorted(this->components_busy.begin(), this->components_busy.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_busy[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentCreateUnitsOnDeath> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_create_units_on_death))
        {
            list_remove(this->components_create_units_on_death, this->map_components_create_units_on_death.at(component->id));
        }
        auto iter = std::lower_bound(this->components_create_units_on_death.begin(), this->components_create_units_on_death.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_create_units_on_death.insert(iter, component);
        assert(std::is_sorted(this->components_create_units_on_death.begin(), this->components_create_units_on_death.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_create_units_on_death[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentAura> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_aura))
        {
            list_remove(this->components_aura, this->map_components_aura.at(component->id));
        }
        auto iter = std::lower_bound(this->components_aura.begin(), this->components_aura.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_aura.insert(iter, component);
        assert(std::is_sorted(this->components_aura.begin(), this->components_aura.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_aura[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentAuraOfSlow> component, int component_id)
    {
        this->add<ComponentAura>(intrusive_ptr<ComponentAura>(component), component_id);
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_aura_of_slow))
        {
            list_remove(this->components_aura_of_slow, this->map_components_aura_of_slow.at(component->id));
        }
        auto iter = std::lower_bound(this->components_aura_of_slow.begin(), this->components_aura_of_slow.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_aura_of_slow.insert(iter, component);
        assert(std::is_sorted(this->components_aura_of_slow.begin(), this->components_aura_of_slow.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_aura_of_slow[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentAuraDamage> component, int component_id)
    {
        this->add<ComponentAura>(intrusive_ptr<ComponentAura>(component), component_id);
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_aura_damage))
        {
            list_remove(this->components_aura_damage, this->map_components_aura_damage.at(component->id));
        }
        auto iter = std::lower_bound(this->components_aura_damage.begin(), this->components_aura_damage.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_aura_damage.insert(iter, component);
        assert(std::is_sorted(this->components_aura_damage.begin(), this->components_aura_damage.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_aura_damage[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSpirit> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_spirit))
        {
            list_remove(this->components_spirit, this->map_components_spirit.at(component->id));
        }
        auto iter = std::lower_bound(this->components_spirit.begin(), this->components_spirit.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_spirit.insert(iter, component);
        assert(std::is_sorted(this->components_spirit.begin(), this->components_spirit.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_spirit[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSpiritBasePoints> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_spirit_base_points))
        {
            list_remove(this->components_spirit_base_points, this->map_components_spirit_base_points.at(component->id));
        }
        auto iter = std::lower_bound(this->components_spirit_base_points.begin(), this->components_spirit_base_points.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_spirit_base_points.insert(iter, component);
        assert(std::is_sorted(this->components_spirit_base_points.begin(), this->components_spirit_base_points.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_spirit_base_points[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSpawnSpirit> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_spawn_spirit))
        {
            list_remove(this->components_spawn_spirit, this->map_components_spawn_spirit.at(component->id));
        }
        auto iter = std::lower_bound(this->components_spawn_spirit.begin(), this->components_spawn_spirit.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_spawn_spirit.insert(iter, component);
        assert(std::is_sorted(this->components_spawn_spirit.begin(), this->components_spawn_spirit.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_spawn_spirit[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentRecharge> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_recharge))
        {
            list_remove(this->components_recharge, this->map_components_recharge.at(component->id));
        }
        auto iter = std::lower_bound(this->components_recharge.begin(), this->components_recharge.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_recharge.insert(iter, component);
        assert(std::is_sorted(this->components_recharge.begin(), this->components_recharge.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_recharge[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentBullet> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_bullet))
        {
            list_remove(this->components_bullet, this->map_components_bullet.at(component->id));
        }
        auto iter = std::lower_bound(this->components_bullet.begin(), this->components_bullet.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_bullet.insert(iter, component);
        assert(std::is_sorted(this->components_bullet.begin(), this->components_bullet.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_bullet[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentBulletLaser> component, int component_id)
    {
        this->add<ComponentBullet>(intrusive_ptr<ComponentBullet>(component), component_id);
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_bullet_laser))
        {
            list_remove(this->components_bullet_laser, this->map_components_bullet_laser.at(component->id));
        }
        auto iter = std::lower_bound(this->components_bullet_laser.begin(), this->components_bullet_laser.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_bullet_laser.insert(iter, component);
        assert(std::is_sorted(this->components_bullet_laser.begin(), this->components_bullet_laser.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_bullet_laser[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentShootBullet> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_shoot_bullet))
        {
            list_remove(this->components_shoot_bullet, this->map_components_shoot_bullet.at(component->id));
        }
        auto iter = std::lower_bound(this->components_shoot_bullet.begin(), this->components_shoot_bullet.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_shoot_bullet.insert(iter, component);
        assert(std::is_sorted(this->components_shoot_bullet.begin(), this->components_shoot_bullet.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_shoot_bullet[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentMeleeAttack> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_melee_attack))
        {
            list_remove(this->components_melee_attack, this->map_components_melee_attack.at(component->id));
        }
        auto iter = std::lower_bound(this->components_melee_attack.begin(), this->components_melee_attack.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_melee_attack.insert(iter, component);
        assert(std::is_sorted(this->components_melee_attack.begin(), this->components_melee_attack.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_melee_attack[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentTargetHighlight> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_target_highlight))
        {
            list_remove(this->components_target_highlight, this->map_components_target_highlight.at(component->id));
        }
        auto iter = std::lower_bound(this->components_target_highlight.begin(), this->components_target_highlight.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_target_highlight.insert(iter, component);
        assert(std::is_sorted(this->components_target_highlight.begin(), this->components_target_highlight.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_target_highlight[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentBulletSplit> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_bullet_split))
        {
            list_remove(this->components_bullet_split, this->map_components_bullet_split.at(component->id));
        }
        auto iter = std::lower_bound(this->components_bullet_split.begin(), this->components_bullet_split.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_bullet_split.insert(iter, component);
        assert(std::is_sorted(this->components_bullet_split.begin(), this->components_bullet_split.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_bullet_split[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentShield> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_shield))
        {
            list_remove(this->components_shield, this->map_components_shield.at(component->id));
        }
        auto iter = std::lower_bound(this->components_shield.begin(), this->components_shield.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_shield.insert(iter, component);
        assert(std::is_sorted(this->components_shield.begin(), this->components_shield.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_shield[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentEffects> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_effects))
        {
            list_remove(this->components_effects, this->map_components_effects.at(component->id));
        }
        auto iter = std::lower_bound(this->components_effects.begin(), this->components_effects.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_effects.insert(iter, component);
        assert(std::is_sorted(this->components_effects.begin(), this->components_effects.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_effects[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentBulletFollowToTarget> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_bullet_follow_to_target))
        {
            list_remove(this->components_bullet_follow_to_target, this->map_components_bullet_follow_to_target.at(component->id));
        }
        auto iter = std::lower_bound(this->components_bullet_follow_to_target.begin(), this->components_bullet_follow_to_target.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_bullet_follow_to_target.insert(iter, component);
        assert(std::is_sorted(this->components_bullet_follow_to_target.begin(), this->components_bullet_follow_to_target.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_bullet_follow_to_target[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentMeteorPeriodic> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_meteor_periodic))
        {
            list_remove(this->components_meteor_periodic, this->map_components_meteor_periodic.at(component->id));
        }
        auto iter = std::lower_bound(this->components_meteor_periodic.begin(), this->components_meteor_periodic.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_meteor_periodic.insert(iter, component);
        assert(std::is_sorted(this->components_meteor_periodic.begin(), this->components_meteor_periodic.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_meteor_periodic[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ElectricDamage> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_electric_damage))
        {
            list_remove(this->components_electric_damage, this->map_components_electric_damage.at(component->id));
        }
        auto iter = std::lower_bound(this->components_electric_damage.begin(), this->components_electric_damage.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_electric_damage.insert(iter, component);
        assert(std::is_sorted(this->components_electric_damage.begin(), this->components_electric_damage.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_electric_damage[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSphereSpawn> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_sphere_spawn))
        {
            list_remove(this->components_sphere_spawn, this->map_components_sphere_spawn.at(component->id));
        }
        auto iter = std::lower_bound(this->components_sphere_spawn.begin(), this->components_sphere_spawn.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_sphere_spawn.insert(iter, component);
        assert(std::is_sorted(this->components_sphere_spawn.begin(), this->components_sphere_spawn.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_sphere_spawn[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSphere> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_sphere))
        {
            list_remove(this->components_sphere, this->map_components_sphere.at(component->id));
        }
        auto iter = std::lower_bound(this->components_sphere.begin(), this->components_sphere.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_sphere.insert(iter, component);
        assert(std::is_sorted(this->components_sphere.begin(), this->components_sphere.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_sphere[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentActions> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_actions))
        {
            list_remove(this->components_actions, this->map_components_actions.at(component->id));
        }
        auto iter = std::lower_bound(this->components_actions.begin(), this->components_actions.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_actions.insert(iter, component);
        assert(std::is_sorted(this->components_actions.begin(), this->components_actions.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_actions[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentGate> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_gate))
        {
            list_remove(this->components_gate, this->map_components_gate.at(component->id));
        }
        auto iter = std::lower_bound(this->components_gate.begin(), this->components_gate.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_gate.insert(iter, component);
        assert(std::is_sorted(this->components_gate.begin(), this->components_gate.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_gate[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentLevelUp> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_level_up))
        {
            list_remove(this->components_level_up, this->map_components_level_up.at(component->id));
        }
        auto iter = std::lower_bound(this->components_level_up.begin(), this->components_level_up.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_level_up.insert(iter, component);
        assert(std::is_sorted(this->components_level_up.begin(), this->components_level_up.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_level_up[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentExp> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_exp))
        {
            list_remove(this->components_exp, this->map_components_exp.at(component->id));
        }
        auto iter = std::lower_bound(this->components_exp.begin(), this->components_exp.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_exp.insert(iter, component);
        assert(std::is_sorted(this->components_exp.begin(), this->components_exp.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_exp[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentExpDrop> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_exp_drop))
        {
            list_remove(this->components_exp_drop, this->map_components_exp_drop.at(component->id));
        }
        auto iter = std::lower_bound(this->components_exp_drop.begin(), this->components_exp_drop.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_exp_drop.insert(iter, component);
        assert(std::is_sorted(this->components_exp_drop.begin(), this->components_exp_drop.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_exp_drop[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentStun> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_stun))
        {
            list_remove(this->components_stun, this->map_components_stun.at(component->id));
        }
        auto iter = std::lower_bound(this->components_stun.begin(), this->components_stun.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_stun.insert(iter, component);
        assert(std::is_sorted(this->components_stun.begin(), this->components_stun.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_stun[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<FireDamage> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_fire_damage))
        {
            list_remove(this->components_fire_damage, this->map_components_fire_damage.at(component->id));
        }
        auto iter = std::lower_bound(this->components_fire_damage.begin(), this->components_fire_damage.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_fire_damage.insert(iter, component);
        assert(std::is_sorted(this->components_fire_damage.begin(), this->components_fire_damage.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_fire_damage[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentMovement> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_movement))
        {
            list_remove(this->components_movement, this->map_components_movement.at(component->id));
        }
        auto iter = std::lower_bound(this->components_movement.begin(), this->components_movement.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_movement.insert(iter, component);
        assert(std::is_sorted(this->components_movement.begin(), this->components_movement.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_movement[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<MoveDirection> component, int component_id)
    {
        this->add<ComponentMovement>(intrusive_ptr<ComponentMovement>(component), component_id);
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_move_direction))
        {
            list_remove(this->components_move_direction, this->map_components_move_direction.at(component->id));
        }
        auto iter = std::lower_bound(this->components_move_direction.begin(), this->components_move_direction.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_move_direction.insert(iter, component);
        assert(std::is_sorted(this->components_move_direction.begin(), this->components_move_direction.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_move_direction[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<MoveToTarget> component, int component_id)
    {
        this->add<ComponentMovement>(intrusive_ptr<ComponentMovement>(component), component_id);
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_move_to_target))
        {
            list_remove(this->components_move_to_target, this->map_components_move_to_target.at(component->id));
        }
        auto iter = std::lower_bound(this->components_move_to_target.begin(), this->components_move_to_target.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_move_to_target.insert(iter, component);
        assert(std::is_sorted(this->components_move_to_target.begin(), this->components_move_to_target.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_move_to_target[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<MoveInstant> component, int component_id)
    {
        this->add<ComponentMovement>(intrusive_ptr<ComponentMovement>(component), component_id);
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_move_instant))
        {
            list_remove(this->components_move_instant, this->map_components_move_instant.at(component->id));
        }
        auto iter = std::lower_bound(this->components_move_instant.begin(), this->components_move_instant.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_move_instant.insert(iter, component);
        assert(std::is_sorted(this->components_move_instant.begin(), this->components_move_instant.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_move_instant[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<MoveVertical> component, int component_id)
    {
        this->add<ComponentMovement>(intrusive_ptr<ComponentMovement>(component), component_id);
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_move_vertical))
        {
            list_remove(this->components_move_vertical, this->map_components_move_vertical.at(component->id));
        }
        auto iter = std::lower_bound(this->components_move_vertical.begin(), this->components_move_vertical.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_move_vertical.insert(iter, component);
        assert(std::is_sorted(this->components_move_vertical.begin(), this->components_move_vertical.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_move_vertical[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<Freezing> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_freezing))
        {
            list_remove(this->components_freezing, this->map_components_freezing.at(component->id));
        }
        auto iter = std::lower_bound(this->components_freezing.begin(), this->components_freezing.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_freezing.insert(iter, component);
        assert(std::is_sorted(this->components_freezing.begin(), this->components_freezing.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_freezing[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<MoveParabolic> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_move_parabolic))
        {
            list_remove(this->components_move_parabolic, this->map_components_move_parabolic.at(component->id));
        }
        auto iter = std::lower_bound(this->components_move_parabolic.begin(), this->components_move_parabolic.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_move_parabolic.insert(iter, component);
        assert(std::is_sorted(this->components_move_parabolic.begin(), this->components_move_parabolic.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_move_parabolic[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentCreateMovementToHero> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_create_movement_to_hero))
        {
            list_remove(this->components_create_movement_to_hero, this->map_components_create_movement_to_hero.at(component->id));
        }
        auto iter = std::lower_bound(this->components_create_movement_to_hero.begin(), this->components_create_movement_to_hero.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_create_movement_to_hero.insert(iter, component);
        assert(std::is_sorted(this->components_create_movement_to_hero.begin(), this->components_create_movement_to_hero.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_create_movement_to_hero[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentPush> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_push))
        {
            list_remove(this->components_push, this->map_components_push.at(component->id));
        }
        auto iter = std::lower_bound(this->components_push.begin(), this->components_push.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_push.insert(iter, component);
        assert(std::is_sorted(this->components_push.begin(), this->components_push.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_push[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSpawn> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_spawn))
        {
            list_remove(this->components_spawn, this->map_components_spawn.at(component->id));
        }
        auto iter = std::lower_bound(this->components_spawn.begin(), this->components_spawn.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_spawn.insert(iter, component);
        assert(std::is_sorted(this->components_spawn.begin(), this->components_spawn.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_spawn[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentHealingOnChangeMaxHp> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_healing_on_change_max_hp))
        {
            list_remove(this->components_healing_on_change_max_hp, this->map_components_healing_on_change_max_hp.at(component->id));
        }
        auto iter = std::lower_bound(this->components_healing_on_change_max_hp.begin(), this->components_healing_on_change_max_hp.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_healing_on_change_max_hp.insert(iter, component);
        assert(std::is_sorted(this->components_healing_on_change_max_hp.begin(), this->components_healing_on_change_max_hp.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_healing_on_change_max_hp[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentDropHeart> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_drop_heart))
        {
            list_remove(this->components_drop_heart, this->map_components_drop_heart.at(component->id));
        }
        auto iter = std::lower_bound(this->components_drop_heart.begin(), this->components_drop_heart.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_drop_heart.insert(iter, component);
        assert(std::is_sorted(this->components_drop_heart.begin(), this->components_drop_heart.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_drop_heart[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentHeart> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_heart))
        {
            list_remove(this->components_heart, this->map_components_heart.at(component->id));
        }
        auto iter = std::lower_bound(this->components_heart.begin(), this->components_heart.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_heart.insert(iter, component);
        assert(std::is_sorted(this->components_heart.begin(), this->components_heart.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_heart[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentHeartAddStats> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_heart_add_stats))
        {
            list_remove(this->components_heart_add_stats, this->map_components_heart_add_stats.at(component->id));
        }
        auto iter = std::lower_bound(this->components_heart_add_stats.begin(), this->components_heart_add_stats.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_heart_add_stats.insert(iter, component);
        assert(std::is_sorted(this->components_heart_add_stats.begin(), this->components_heart_add_stats.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_heart_add_stats[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentHeartAddStatsHp> component, int component_id)
    {
        this->add<ComponentHeartAddStats>(intrusive_ptr<ComponentHeartAddStats>(component), component_id);
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_heart_add_stats_hp))
        {
            list_remove(this->components_heart_add_stats_hp, this->map_components_heart_add_stats_hp.at(component->id));
        }
        auto iter = std::lower_bound(this->components_heart_add_stats_hp.begin(), this->components_heart_add_stats_hp.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_heart_add_stats_hp.insert(iter, component);
        assert(std::is_sorted(this->components_heart_add_stats_hp.begin(), this->components_heart_add_stats_hp.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_heart_add_stats_hp[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentHeartAddStatsDamage> component, int component_id)
    {
        this->add<ComponentHeartAddStats>(intrusive_ptr<ComponentHeartAddStats>(component), component_id);
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_heart_add_stats_damage))
        {
            list_remove(this->components_heart_add_stats_damage, this->map_components_heart_add_stats_damage.at(component->id));
        }
        auto iter = std::lower_bound(this->components_heart_add_stats_damage.begin(), this->components_heart_add_stats_damage.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_heart_add_stats_damage.insert(iter, component);
        assert(std::is_sorted(this->components_heart_add_stats_damage.begin(), this->components_heart_add_stats_damage.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_heart_add_stats_damage[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentRandomHealing> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_random_healing))
        {
            list_remove(this->components_random_healing, this->map_components_random_healing.at(component->id));
        }
        auto iter = std::lower_bound(this->components_random_healing.begin(), this->components_random_healing.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_random_healing.insert(iter, component);
        assert(std::is_sorted(this->components_random_healing.begin(), this->components_random_healing.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_random_healing[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentVampire> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_vampire))
        {
            list_remove(this->components_vampire, this->map_components_vampire.at(component->id));
        }
        auto iter = std::lower_bound(this->components_vampire.begin(), this->components_vampire.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_vampire.insert(iter, component);
        assert(std::is_sorted(this->components_vampire.begin(), this->components_vampire.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_vampire[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentHealingLowHp> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_healing_low_hp))
        {
            list_remove(this->components_healing_low_hp, this->map_components_healing_low_hp.at(component->id));
        }
        auto iter = std::lower_bound(this->components_healing_low_hp.begin(), this->components_healing_low_hp.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_healing_low_hp.insert(iter, component);
        assert(std::is_sorted(this->components_healing_low_hp.begin(), this->components_healing_low_hp.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_healing_low_hp[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentBody> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_body))
        {
            list_remove(this->components_body, this->map_components_body.at(component->id));
        }
        auto iter = std::lower_bound(this->components_body.begin(), this->components_body.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_body.insert(iter, component);
        assert(std::is_sorted(this->components_body.begin(), this->components_body.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_body[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSwordCast> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_sword_cast))
        {
            list_remove(this->components_sword_cast, this->map_components_sword_cast.at(component->id));
        }
        auto iter = std::lower_bound(this->components_sword_cast.begin(), this->components_sword_cast.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_sword_cast.insert(iter, component);
        assert(std::is_sorted(this->components_sword_cast.begin(), this->components_sword_cast.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_sword_cast[component->id] = component;
    }

    template<> void ModelEcsBase::add(intrusive_ptr<ComponentSword> component, int component_id)
    {
        assert(component->id == 0 || component->id == component_id || component_id == 0);
        if(component_id != 0)
        {
            component->id = component_id;
        }
        assert(component->id > 0);
        if(in_map(component->id, this->map_components_sword))
        {
            list_remove(this->components_sword, this->map_components_sword.at(component->id));
        }
        auto iter = std::lower_bound(this->components_sword.begin(), this->components_sword.end(), component, [](const auto& a, const auto& b)
        {
            return a->id < b->id;
        });
        this->components_sword.insert(iter, component);
        assert(std::is_sorted(this->components_sword.begin(), this->components_sword.end(), [](const auto& l, const auto& r)
        {
            return l->id < r->id;
        }));
        this->map_components_sword[component->id] = component;
    }

    template<> void ModelEcsBase::add(ComponentData* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentData>(component), component_id);
    }
    template<> void ModelEcsBase::add(Transform* component, int component_id)
    {
        this->add(intrusive_ptr<Transform>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentStats* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentStats>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSide* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSide>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentTargetable* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentTargetable>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentHealth* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentHealth>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentDamage* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentDamage>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentUser* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentUser>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSpineInfo* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSpineInfo>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentBusy* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentBusy>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentCreateUnitsOnDeath* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentCreateUnitsOnDeath>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentAura* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentAura>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentAuraOfSlow* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentAuraOfSlow>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentAuraDamage* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentAuraDamage>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSpirit* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSpirit>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSpiritBasePoints* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSpiritBasePoints>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSpawnSpirit* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSpawnSpirit>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentRecharge* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentRecharge>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentBullet* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentBullet>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentBulletLaser* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentBulletLaser>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentShootBullet* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentShootBullet>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentMeleeAttack* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentMeleeAttack>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentTargetHighlight* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentTargetHighlight>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentBulletSplit* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentBulletSplit>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentShield* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentShield>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentEffects* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentEffects>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentBulletFollowToTarget* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentBulletFollowToTarget>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentMeteorPeriodic* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentMeteorPeriodic>(component), component_id);
    }
    template<> void ModelEcsBase::add(ElectricDamage* component, int component_id)
    {
        this->add(intrusive_ptr<ElectricDamage>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSphereSpawn* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSphereSpawn>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSphere* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSphere>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentActions* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentActions>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentGate* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentGate>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentLevelUp* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentLevelUp>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentExp* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentExp>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentExpDrop* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentExpDrop>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentStun* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentStun>(component), component_id);
    }
    template<> void ModelEcsBase::add(FireDamage* component, int component_id)
    {
        this->add(intrusive_ptr<FireDamage>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentMovement* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentMovement>(component), component_id);
    }
    template<> void ModelEcsBase::add(MoveDirection* component, int component_id)
    {
        this->add(intrusive_ptr<MoveDirection>(component), component_id);
    }
    template<> void ModelEcsBase::add(MoveToTarget* component, int component_id)
    {
        this->add(intrusive_ptr<MoveToTarget>(component), component_id);
    }
    template<> void ModelEcsBase::add(MoveInstant* component, int component_id)
    {
        this->add(intrusive_ptr<MoveInstant>(component), component_id);
    }
    template<> void ModelEcsBase::add(MoveVertical* component, int component_id)
    {
        this->add(intrusive_ptr<MoveVertical>(component), component_id);
    }
    template<> void ModelEcsBase::add(Freezing* component, int component_id)
    {
        this->add(intrusive_ptr<Freezing>(component), component_id);
    }
    template<> void ModelEcsBase::add(MoveParabolic* component, int component_id)
    {
        this->add(intrusive_ptr<MoveParabolic>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentCreateMovementToHero* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentCreateMovementToHero>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentPush* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentPush>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSpawn* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSpawn>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentHealingOnChangeMaxHp* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentHealingOnChangeMaxHp>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentDropHeart* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentDropHeart>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentHeart* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentHeart>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentHeartAddStats* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentHeartAddStats>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentHeartAddStatsHp* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentHeartAddStatsHp>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentHeartAddStatsDamage* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentHeartAddStatsDamage>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentRandomHealing* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentRandomHealing>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentVampire* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentVampire>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentHealingLowHp* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentHealingLowHp>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentBody* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentBody>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSwordCast* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSwordCast>(component), component_id);
    }
    template<> void ModelEcsBase::add(ComponentSword* component, int component_id)
    {
        this->add(intrusive_ptr<ComponentSword>(component), component_id);
    }
    void ModelEcsBase::copy_entity_from_model(ModelEcsBase* model, int id, int new_id)
    {
        auto component_data = model->get<ComponentData>(id);
        if(component_data)
        {
            auto clone = clone_object<ComponentData>(component_data);
            clone->id = new_id;
            this->add<ComponentData>(clone, new_id);
        }
        auto component_transform = model->get<Transform>(id);
        if(component_transform)
        {
            auto clone = clone_object<Transform>(component_transform);
            clone->id = new_id;
            this->add<Transform>(clone, new_id);
        }
        auto component_stats = model->get<ComponentStats>(id);
        if(component_stats)
        {
            auto clone = clone_object<ComponentStats>(component_stats);
            clone->id = new_id;
            this->add<ComponentStats>(clone, new_id);
        }
        auto component_side = model->get<ComponentSide>(id);
        if(component_side)
        {
            auto clone = clone_object<ComponentSide>(component_side);
            clone->id = new_id;
            this->add<ComponentSide>(clone, new_id);
        }
        auto component_targetable = model->get<ComponentTargetable>(id);
        if(component_targetable)
        {
            auto clone = clone_object<ComponentTargetable>(component_targetable);
            clone->id = new_id;
            this->add<ComponentTargetable>(clone, new_id);
        }
        auto component_health = model->get<ComponentHealth>(id);
        if(component_health)
        {
            auto clone = clone_object<ComponentHealth>(component_health);
            clone->id = new_id;
            this->add<ComponentHealth>(clone, new_id);
        }
        auto component_damage = model->get<ComponentDamage>(id);
        if(component_damage)
        {
            auto clone = clone_object<ComponentDamage>(component_damage);
            clone->id = new_id;
            this->add<ComponentDamage>(clone, new_id);
        }
        auto component_user = model->get<ComponentUser>(id);
        if(component_user)
        {
            auto clone = clone_object<ComponentUser>(component_user);
            clone->id = new_id;
            this->add<ComponentUser>(clone, new_id);
        }
        auto component_spine_info = model->get<ComponentSpineInfo>(id);
        if(component_spine_info)
        {
            auto clone = clone_object<ComponentSpineInfo>(component_spine_info);
            clone->id = new_id;
            this->add<ComponentSpineInfo>(clone, new_id);
        }
        auto component_busy = model->get<ComponentBusy>(id);
        if(component_busy)
        {
            auto clone = clone_object<ComponentBusy>(component_busy);
            clone->id = new_id;
            this->add<ComponentBusy>(clone, new_id);
        }
        auto component_create_units_on_death = model->get<ComponentCreateUnitsOnDeath>(id);
        if(component_create_units_on_death)
        {
            auto clone = clone_object<ComponentCreateUnitsOnDeath>(component_create_units_on_death);
            clone->id = new_id;
            this->add<ComponentCreateUnitsOnDeath>(clone, new_id);
        }
        auto component_aura = model->get<ComponentAura>(id);
        if(component_aura)
        {
            auto clone = clone_object<ComponentAura>(component_aura);
            clone->id = new_id;
            this->add<ComponentAura>(clone, new_id);
        }
        auto component_aura_of_slow = model->get<ComponentAuraOfSlow>(id);
        if(component_aura_of_slow)
        {
            auto clone = clone_object<ComponentAuraOfSlow>(component_aura_of_slow);
            clone->id = new_id;
            this->add<ComponentAuraOfSlow>(clone, new_id);
        }
        auto component_aura_damage = model->get<ComponentAuraDamage>(id);
        if(component_aura_damage)
        {
            auto clone = clone_object<ComponentAuraDamage>(component_aura_damage);
            clone->id = new_id;
            this->add<ComponentAuraDamage>(clone, new_id);
        }
        auto component_spirit = model->get<ComponentSpirit>(id);
        if(component_spirit)
        {
            auto clone = clone_object<ComponentSpirit>(component_spirit);
            clone->id = new_id;
            this->add<ComponentSpirit>(clone, new_id);
        }
        auto component_spirit_base_points = model->get<ComponentSpiritBasePoints>(id);
        if(component_spirit_base_points)
        {
            auto clone = clone_object<ComponentSpiritBasePoints>(component_spirit_base_points);
            clone->id = new_id;
            this->add<ComponentSpiritBasePoints>(clone, new_id);
        }
        auto component_spawn_spirit = model->get<ComponentSpawnSpirit>(id);
        if(component_spawn_spirit)
        {
            auto clone = clone_object<ComponentSpawnSpirit>(component_spawn_spirit);
            clone->id = new_id;
            this->add<ComponentSpawnSpirit>(clone, new_id);
        }
        auto component_recharge = model->get<ComponentRecharge>(id);
        if(component_recharge)
        {
            auto clone = clone_object<ComponentRecharge>(component_recharge);
            clone->id = new_id;
            this->add<ComponentRecharge>(clone, new_id);
        }
        auto component_bullet = model->get<ComponentBullet>(id);
        if(component_bullet)
        {
            auto clone = clone_object<ComponentBullet>(component_bullet);
            clone->id = new_id;
            this->add<ComponentBullet>(clone, new_id);
        }
        auto component_bullet_laser = model->get<ComponentBulletLaser>(id);
        if(component_bullet_laser)
        {
            auto clone = clone_object<ComponentBulletLaser>(component_bullet_laser);
            clone->id = new_id;
            this->add<ComponentBulletLaser>(clone, new_id);
        }
        auto component_shoot_bullet = model->get<ComponentShootBullet>(id);
        if(component_shoot_bullet)
        {
            auto clone = clone_object<ComponentShootBullet>(component_shoot_bullet);
            clone->id = new_id;
            this->add<ComponentShootBullet>(clone, new_id);
        }
        auto component_melee_attack = model->get<ComponentMeleeAttack>(id);
        if(component_melee_attack)
        {
            auto clone = clone_object<ComponentMeleeAttack>(component_melee_attack);
            clone->id = new_id;
            this->add<ComponentMeleeAttack>(clone, new_id);
        }
        auto component_target_highlight = model->get<ComponentTargetHighlight>(id);
        if(component_target_highlight)
        {
            auto clone = clone_object<ComponentTargetHighlight>(component_target_highlight);
            clone->id = new_id;
            this->add<ComponentTargetHighlight>(clone, new_id);
        }
        auto component_bullet_split = model->get<ComponentBulletSplit>(id);
        if(component_bullet_split)
        {
            auto clone = clone_object<ComponentBulletSplit>(component_bullet_split);
            clone->id = new_id;
            this->add<ComponentBulletSplit>(clone, new_id);
        }
        auto component_shield = model->get<ComponentShield>(id);
        if(component_shield)
        {
            auto clone = clone_object<ComponentShield>(component_shield);
            clone->id = new_id;
            this->add<ComponentShield>(clone, new_id);
        }
        auto component_effects = model->get<ComponentEffects>(id);
        if(component_effects)
        {
            auto clone = clone_object<ComponentEffects>(component_effects);
            clone->id = new_id;
            this->add<ComponentEffects>(clone, new_id);
        }
        auto component_bullet_follow_to_target = model->get<ComponentBulletFollowToTarget>(id);
        if(component_bullet_follow_to_target)
        {
            auto clone = clone_object<ComponentBulletFollowToTarget>(component_bullet_follow_to_target);
            clone->id = new_id;
            this->add<ComponentBulletFollowToTarget>(clone, new_id);
        }
        auto component_meteor_periodic = model->get<ComponentMeteorPeriodic>(id);
        if(component_meteor_periodic)
        {
            auto clone = clone_object<ComponentMeteorPeriodic>(component_meteor_periodic);
            clone->id = new_id;
            this->add<ComponentMeteorPeriodic>(clone, new_id);
        }
        auto component_electric_damage = model->get<ElectricDamage>(id);
        if(component_electric_damage)
        {
            auto clone = clone_object<ElectricDamage>(component_electric_damage);
            clone->id = new_id;
            this->add<ElectricDamage>(clone, new_id);
        }
        auto component_sphere_spawn = model->get<ComponentSphereSpawn>(id);
        if(component_sphere_spawn)
        {
            auto clone = clone_object<ComponentSphereSpawn>(component_sphere_spawn);
            clone->id = new_id;
            this->add<ComponentSphereSpawn>(clone, new_id);
        }
        auto component_sphere = model->get<ComponentSphere>(id);
        if(component_sphere)
        {
            auto clone = clone_object<ComponentSphere>(component_sphere);
            clone->id = new_id;
            this->add<ComponentSphere>(clone, new_id);
        }
        auto component_actions = model->get<ComponentActions>(id);
        if(component_actions)
        {
            auto clone = clone_object<ComponentActions>(component_actions);
            clone->id = new_id;
            this->add<ComponentActions>(clone, new_id);
        }
        auto component_gate = model->get<ComponentGate>(id);
        if(component_gate)
        {
            auto clone = clone_object<ComponentGate>(component_gate);
            clone->id = new_id;
            this->add<ComponentGate>(clone, new_id);
        }
        auto component_level_up = model->get<ComponentLevelUp>(id);
        if(component_level_up)
        {
            auto clone = clone_object<ComponentLevelUp>(component_level_up);
            clone->id = new_id;
            this->add<ComponentLevelUp>(clone, new_id);
        }
        auto component_exp = model->get<ComponentExp>(id);
        if(component_exp)
        {
            auto clone = clone_object<ComponentExp>(component_exp);
            clone->id = new_id;
            this->add<ComponentExp>(clone, new_id);
        }
        auto component_exp_drop = model->get<ComponentExpDrop>(id);
        if(component_exp_drop)
        {
            auto clone = clone_object<ComponentExpDrop>(component_exp_drop);
            clone->id = new_id;
            this->add<ComponentExpDrop>(clone, new_id);
        }
        auto component_stun = model->get<ComponentStun>(id);
        if(component_stun)
        {
            auto clone = clone_object<ComponentStun>(component_stun);
            clone->id = new_id;
            this->add<ComponentStun>(clone, new_id);
        }
        auto component_fire_damage = model->get<FireDamage>(id);
        if(component_fire_damage)
        {
            auto clone = clone_object<FireDamage>(component_fire_damage);
            clone->id = new_id;
            this->add<FireDamage>(clone, new_id);
        }
        auto component_movement = model->get<ComponentMovement>(id);
        if(component_movement)
        {
            auto clone = clone_object<ComponentMovement>(component_movement);
            clone->id = new_id;
            this->add<ComponentMovement>(clone, new_id);
        }
        auto component_move_direction = model->get<MoveDirection>(id);
        if(component_move_direction)
        {
            auto clone = clone_object<MoveDirection>(component_move_direction);
            clone->id = new_id;
            this->add<MoveDirection>(clone, new_id);
        }
        auto component_move_to_target = model->get<MoveToTarget>(id);
        if(component_move_to_target)
        {
            auto clone = clone_object<MoveToTarget>(component_move_to_target);
            clone->id = new_id;
            this->add<MoveToTarget>(clone, new_id);
        }
        auto component_move_instant = model->get<MoveInstant>(id);
        if(component_move_instant)
        {
            auto clone = clone_object<MoveInstant>(component_move_instant);
            clone->id = new_id;
            this->add<MoveInstant>(clone, new_id);
        }
        auto component_move_vertical = model->get<MoveVertical>(id);
        if(component_move_vertical)
        {
            auto clone = clone_object<MoveVertical>(component_move_vertical);
            clone->id = new_id;
            this->add<MoveVertical>(clone, new_id);
        }
        auto component_freezing = model->get<Freezing>(id);
        if(component_freezing)
        {
            auto clone = clone_object<Freezing>(component_freezing);
            clone->id = new_id;
            this->add<Freezing>(clone, new_id);
        }
        auto component_move_parabolic = model->get<MoveParabolic>(id);
        if(component_move_parabolic)
        {
            auto clone = clone_object<MoveParabolic>(component_move_parabolic);
            clone->id = new_id;
            this->add<MoveParabolic>(clone, new_id);
        }
        auto component_create_movement_to_hero = model->get<ComponentCreateMovementToHero>(id);
        if(component_create_movement_to_hero)
        {
            auto clone = clone_object<ComponentCreateMovementToHero>(component_create_movement_to_hero);
            clone->id = new_id;
            this->add<ComponentCreateMovementToHero>(clone, new_id);
        }
        auto component_push = model->get<ComponentPush>(id);
        if(component_push)
        {
            auto clone = clone_object<ComponentPush>(component_push);
            clone->id = new_id;
            this->add<ComponentPush>(clone, new_id);
        }
        auto component_spawn = model->get<ComponentSpawn>(id);
        if(component_spawn)
        {
            auto clone = clone_object<ComponentSpawn>(component_spawn);
            clone->id = new_id;
            this->add<ComponentSpawn>(clone, new_id);
        }
        auto component_healing_on_change_max_hp = model->get<ComponentHealingOnChangeMaxHp>(id);
        if(component_healing_on_change_max_hp)
        {
            auto clone = clone_object<ComponentHealingOnChangeMaxHp>(component_healing_on_change_max_hp);
            clone->id = new_id;
            this->add<ComponentHealingOnChangeMaxHp>(clone, new_id);
        }
        auto component_drop_heart = model->get<ComponentDropHeart>(id);
        if(component_drop_heart)
        {
            auto clone = clone_object<ComponentDropHeart>(component_drop_heart);
            clone->id = new_id;
            this->add<ComponentDropHeart>(clone, new_id);
        }
        auto component_heart = model->get<ComponentHeart>(id);
        if(component_heart)
        {
            auto clone = clone_object<ComponentHeart>(component_heart);
            clone->id = new_id;
            this->add<ComponentHeart>(clone, new_id);
        }
        auto component_heart_add_stats = model->get<ComponentHeartAddStats>(id);
        if(component_heart_add_stats)
        {
            auto clone = clone_object<ComponentHeartAddStats>(component_heart_add_stats);
            clone->id = new_id;
            this->add<ComponentHeartAddStats>(clone, new_id);
        }
        auto component_heart_add_stats_hp = model->get<ComponentHeartAddStatsHp>(id);
        if(component_heart_add_stats_hp)
        {
            auto clone = clone_object<ComponentHeartAddStatsHp>(component_heart_add_stats_hp);
            clone->id = new_id;
            this->add<ComponentHeartAddStatsHp>(clone, new_id);
        }
        auto component_heart_add_stats_damage = model->get<ComponentHeartAddStatsDamage>(id);
        if(component_heart_add_stats_damage)
        {
            auto clone = clone_object<ComponentHeartAddStatsDamage>(component_heart_add_stats_damage);
            clone->id = new_id;
            this->add<ComponentHeartAddStatsDamage>(clone, new_id);
        }
        auto component_random_healing = model->get<ComponentRandomHealing>(id);
        if(component_random_healing)
        {
            auto clone = clone_object<ComponentRandomHealing>(component_random_healing);
            clone->id = new_id;
            this->add<ComponentRandomHealing>(clone, new_id);
        }
        auto component_vampire = model->get<ComponentVampire>(id);
        if(component_vampire)
        {
            auto clone = clone_object<ComponentVampire>(component_vampire);
            clone->id = new_id;
            this->add<ComponentVampire>(clone, new_id);
        }
        auto component_healing_low_hp = model->get<ComponentHealingLowHp>(id);
        if(component_healing_low_hp)
        {
            auto clone = clone_object<ComponentHealingLowHp>(component_healing_low_hp);
            clone->id = new_id;
            this->add<ComponentHealingLowHp>(clone, new_id);
        }
        auto component_body = model->get<ComponentBody>(id);
        if(component_body)
        {
            auto clone = clone_object<ComponentBody>(component_body);
            clone->id = new_id;
            this->add<ComponentBody>(clone, new_id);
        }
        auto component_sword_cast = model->get<ComponentSwordCast>(id);
        if(component_sword_cast)
        {
            auto clone = clone_object<ComponentSwordCast>(component_sword_cast);
            clone->id = new_id;
            this->add<ComponentSwordCast>(clone, new_id);
        }
        auto component_sword = model->get<ComponentSword>(id);
        if(component_sword)
        {
            auto clone = clone_object<ComponentSword>(component_sword);
            clone->id = new_id;
            this->add<ComponentSword>(clone, new_id);
        }
    }

    void ModelEcsBase::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(next_free_id, "next_free_id", int(1));
        serializer.serialize(player_id, "player_id", int(0));
        serializer.serialize(entities, "entities");
        serializer.serialize(game_timer, "game_timer", float(0.0));
        serializer.serialize(tasks, "tasks");
        serializer.serialize(data, "data");
        serializer.serialize(wave_index, "wave_index", int(0));
        serializer.serialize(ground, "ground");
        serializer.serialize(enemies_level, "enemies_level", int(-1));
        serializer.serialize(enemies_rank, "enemies_rank", int(-1));
        serializer.serialize(wave_finished, "wave_finished", bool(false));
        serializer.serialize(timer_wave_interval, "timer_wave_interval", float(0));
        serializer.serialize(timer_wave_duration, "timer_wave_duration", float(0));
        serializer.serialize(has_skills, "has_skills");
        serializer.serialize(spawn_points, "spawn_points");
        serializer.serialize(components_data, "components_data");
        serializer.serialize(components_transform, "components_transform");
        serializer.serialize(components_stats, "components_stats");
        serializer.serialize(components_side, "components_side");
        serializer.serialize(components_targetable, "components_targetable");
        serializer.serialize(components_health, "components_health");
        serializer.serialize(components_damage, "components_damage");
        serializer.serialize(components_user, "components_user");
        serializer.serialize(components_spine_info, "components_spine_info");
        serializer.serialize(components_busy, "components_busy");
        serializer.serialize(components_create_units_on_death, "components_create_units_on_death");
        serializer.serialize(components_aura, "components_aura");
        serializer.serialize(components_aura_of_slow, "components_aura_of_slow");
        serializer.serialize(components_aura_damage, "components_aura_damage");
        serializer.serialize(components_spirit, "components_spirit");
        serializer.serialize(components_spirit_base_points, "components_spirit_base_points");
        serializer.serialize(components_spawn_spirit, "components_spawn_spirit");
        serializer.serialize(components_recharge, "components_recharge");
        serializer.serialize(components_bullet, "components_bullet");
        serializer.serialize(components_bullet_laser, "components_bullet_laser");
        serializer.serialize(components_shoot_bullet, "components_shoot_bullet");
        serializer.serialize(components_melee_attack, "components_melee_attack");
        serializer.serialize(components_target_highlight, "components_target_highlight");
        serializer.serialize(components_bullet_split, "components_bullet_split");
        serializer.serialize(components_shield, "components_shield");
        serializer.serialize(components_effects, "components_effects");
        serializer.serialize(components_bullet_follow_to_target, "components_bullet_follow_to_target");
        serializer.serialize(components_meteor_periodic, "components_meteor_periodic");
        serializer.serialize(components_electric_damage, "components_electric_damage");
        serializer.serialize(components_sphere_spawn, "components_sphere_spawn");
        serializer.serialize(components_sphere, "components_sphere");
        serializer.serialize(components_actions, "components_actions");
        serializer.serialize(components_gate, "components_gate");
        serializer.serialize(components_level_up, "components_level_up");
        serializer.serialize(components_exp, "components_exp");
        serializer.serialize(components_exp_drop, "components_exp_drop");
        serializer.serialize(components_stun, "components_stun");
        serializer.serialize(components_fire_damage, "components_fire_damage");
        serializer.serialize(components_movement, "components_movement");
        serializer.serialize(components_move_direction, "components_move_direction");
        serializer.serialize(components_move_to_target, "components_move_to_target");
        serializer.serialize(components_move_instant, "components_move_instant");
        serializer.serialize(components_move_vertical, "components_move_vertical");
        serializer.serialize(components_freezing, "components_freezing");
        serializer.serialize(components_move_parabolic, "components_move_parabolic");
        serializer.serialize(components_create_movement_to_hero, "components_create_movement_to_hero");
        serializer.serialize(components_push, "components_push");
        serializer.serialize(components_spawn, "components_spawn");
        serializer.serialize(components_healing_on_change_max_hp, "components_healing_on_change_max_hp");
        serializer.serialize(components_drop_heart, "components_drop_heart");
        serializer.serialize(components_heart, "components_heart");
        serializer.serialize(components_heart_add_stats, "components_heart_add_stats");
        serializer.serialize(components_heart_add_stats_hp, "components_heart_add_stats_hp");
        serializer.serialize(components_heart_add_stats_damage, "components_heart_add_stats_damage");
        serializer.serialize(components_random_healing, "components_random_healing");
        serializer.serialize(components_vampire, "components_vampire");
        serializer.serialize(components_healing_low_hp, "components_healing_low_hp");
        serializer.serialize(components_body, "components_body");
        serializer.serialize(components_sword_cast, "components_sword_cast");
        serializer.serialize(components_sword, "components_sword");
    }

    void ModelEcsBase::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(next_free_id, "next_free_id", int(1));
        deserializer.deserialize(player_id, "player_id", int(0));
        deserializer.deserialize(entities, "entities");
        deserializer.deserialize(game_timer, "game_timer", float(0.0));
        deserializer.deserialize(tasks, "tasks");
        deserializer.deserialize(data, "data");
        deserializer.deserialize(wave_index, "wave_index", int(0));
        deserializer.deserialize(ground, "ground");
        deserializer.deserialize(enemies_level, "enemies_level", int(-1));
        deserializer.deserialize(enemies_rank, "enemies_rank", int(-1));
        deserializer.deserialize(wave_finished, "wave_finished", bool(false));
        deserializer.deserialize(timer_wave_interval, "timer_wave_interval", float(0));
        deserializer.deserialize(timer_wave_duration, "timer_wave_duration", float(0));
        deserializer.deserialize(has_skills, "has_skills");
        deserializer.deserialize(spawn_points, "spawn_points");
        deserializer.deserialize(components_data, "components_data");
        deserializer.deserialize(components_transform, "components_transform");
        deserializer.deserialize(components_stats, "components_stats");
        deserializer.deserialize(components_side, "components_side");
        deserializer.deserialize(components_targetable, "components_targetable");
        deserializer.deserialize(components_health, "components_health");
        deserializer.deserialize(components_damage, "components_damage");
        deserializer.deserialize(components_user, "components_user");
        deserializer.deserialize(components_spine_info, "components_spine_info");
        deserializer.deserialize(components_busy, "components_busy");
        deserializer.deserialize(components_create_units_on_death, "components_create_units_on_death");
        deserializer.deserialize(components_aura, "components_aura");
        deserializer.deserialize(components_aura_of_slow, "components_aura_of_slow");
        deserializer.deserialize(components_aura_damage, "components_aura_damage");
        deserializer.deserialize(components_spirit, "components_spirit");
        deserializer.deserialize(components_spirit_base_points, "components_spirit_base_points");
        deserializer.deserialize(components_spawn_spirit, "components_spawn_spirit");
        deserializer.deserialize(components_recharge, "components_recharge");
        deserializer.deserialize(components_bullet, "components_bullet");
        deserializer.deserialize(components_bullet_laser, "components_bullet_laser");
        deserializer.deserialize(components_shoot_bullet, "components_shoot_bullet");
        deserializer.deserialize(components_melee_attack, "components_melee_attack");
        deserializer.deserialize(components_target_highlight, "components_target_highlight");
        deserializer.deserialize(components_bullet_split, "components_bullet_split");
        deserializer.deserialize(components_shield, "components_shield");
        deserializer.deserialize(components_effects, "components_effects");
        deserializer.deserialize(components_bullet_follow_to_target, "components_bullet_follow_to_target");
        deserializer.deserialize(components_meteor_periodic, "components_meteor_periodic");
        deserializer.deserialize(components_electric_damage, "components_electric_damage");
        deserializer.deserialize(components_sphere_spawn, "components_sphere_spawn");
        deserializer.deserialize(components_sphere, "components_sphere");
        deserializer.deserialize(components_actions, "components_actions");
        deserializer.deserialize(components_gate, "components_gate");
        deserializer.deserialize(components_level_up, "components_level_up");
        deserializer.deserialize(components_exp, "components_exp");
        deserializer.deserialize(components_exp_drop, "components_exp_drop");
        deserializer.deserialize(components_stun, "components_stun");
        deserializer.deserialize(components_fire_damage, "components_fire_damage");
        deserializer.deserialize(components_movement, "components_movement");
        deserializer.deserialize(components_move_direction, "components_move_direction");
        deserializer.deserialize(components_move_to_target, "components_move_to_target");
        deserializer.deserialize(components_move_instant, "components_move_instant");
        deserializer.deserialize(components_move_vertical, "components_move_vertical");
        deserializer.deserialize(components_freezing, "components_freezing");
        deserializer.deserialize(components_move_parabolic, "components_move_parabolic");
        deserializer.deserialize(components_create_movement_to_hero, "components_create_movement_to_hero");
        deserializer.deserialize(components_push, "components_push");
        deserializer.deserialize(components_spawn, "components_spawn");
        deserializer.deserialize(components_healing_on_change_max_hp, "components_healing_on_change_max_hp");
        deserializer.deserialize(components_drop_heart, "components_drop_heart");
        deserializer.deserialize(components_heart, "components_heart");
        deserializer.deserialize(components_heart_add_stats, "components_heart_add_stats");
        deserializer.deserialize(components_heart_add_stats_hp, "components_heart_add_stats_hp");
        deserializer.deserialize(components_heart_add_stats_damage, "components_heart_add_stats_damage");
        deserializer.deserialize(components_random_healing, "components_random_healing");
        deserializer.deserialize(components_vampire, "components_vampire");
        deserializer.deserialize(components_healing_low_hp, "components_healing_low_hp");
        deserializer.deserialize(components_body, "components_body");
        deserializer.deserialize(components_sword_cast, "components_sword_cast");
        deserializer.deserialize(components_sword, "components_sword");
    }

    void ModelEcsBase::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(next_free_id, "next_free_id", int(1));
        serializer.serialize(player_id, "player_id", int(0));
        serializer.serialize(entities, "entities");
        serializer.serialize(game_timer, "game_timer", float(0.0));
        serializer.serialize(tasks, "tasks");
        serializer.serialize(data, "data");
        serializer.serialize(wave_index, "wave_index", int(0));
        serializer.serialize(ground, "ground");
        serializer.serialize(enemies_level, "enemies_level", int(-1));
        serializer.serialize(enemies_rank, "enemies_rank", int(-1));
        serializer.serialize(wave_finished, "wave_finished", bool(false));
        serializer.serialize(timer_wave_interval, "timer_wave_interval", float(0));
        serializer.serialize(timer_wave_duration, "timer_wave_duration", float(0));
        serializer.serialize(has_skills, "has_skills");
        serializer.serialize(spawn_points, "spawn_points");
        serializer.serialize(components_data, "components_data");
        serializer.serialize(components_transform, "components_transform");
        serializer.serialize(components_stats, "components_stats");
        serializer.serialize(components_side, "components_side");
        serializer.serialize(components_targetable, "components_targetable");
        serializer.serialize(components_health, "components_health");
        serializer.serialize(components_damage, "components_damage");
        serializer.serialize(components_user, "components_user");
        serializer.serialize(components_spine_info, "components_spine_info");
        serializer.serialize(components_busy, "components_busy");
        serializer.serialize(components_create_units_on_death, "components_create_units_on_death");
        serializer.serialize(components_aura, "components_aura");
        serializer.serialize(components_aura_of_slow, "components_aura_of_slow");
        serializer.serialize(components_aura_damage, "components_aura_damage");
        serializer.serialize(components_spirit, "components_spirit");
        serializer.serialize(components_spirit_base_points, "components_spirit_base_points");
        serializer.serialize(components_spawn_spirit, "components_spawn_spirit");
        serializer.serialize(components_recharge, "components_recharge");
        serializer.serialize(components_bullet, "components_bullet");
        serializer.serialize(components_bullet_laser, "components_bullet_laser");
        serializer.serialize(components_shoot_bullet, "components_shoot_bullet");
        serializer.serialize(components_melee_attack, "components_melee_attack");
        serializer.serialize(components_target_highlight, "components_target_highlight");
        serializer.serialize(components_bullet_split, "components_bullet_split");
        serializer.serialize(components_shield, "components_shield");
        serializer.serialize(components_effects, "components_effects");
        serializer.serialize(components_bullet_follow_to_target, "components_bullet_follow_to_target");
        serializer.serialize(components_meteor_periodic, "components_meteor_periodic");
        serializer.serialize(components_electric_damage, "components_electric_damage");
        serializer.serialize(components_sphere_spawn, "components_sphere_spawn");
        serializer.serialize(components_sphere, "components_sphere");
        serializer.serialize(components_actions, "components_actions");
        serializer.serialize(components_gate, "components_gate");
        serializer.serialize(components_level_up, "components_level_up");
        serializer.serialize(components_exp, "components_exp");
        serializer.serialize(components_exp_drop, "components_exp_drop");
        serializer.serialize(components_stun, "components_stun");
        serializer.serialize(components_fire_damage, "components_fire_damage");
        serializer.serialize(components_movement, "components_movement");
        serializer.serialize(components_move_direction, "components_move_direction");
        serializer.serialize(components_move_to_target, "components_move_to_target");
        serializer.serialize(components_move_instant, "components_move_instant");
        serializer.serialize(components_move_vertical, "components_move_vertical");
        serializer.serialize(components_freezing, "components_freezing");
        serializer.serialize(components_move_parabolic, "components_move_parabolic");
        serializer.serialize(components_create_movement_to_hero, "components_create_movement_to_hero");
        serializer.serialize(components_push, "components_push");
        serializer.serialize(components_spawn, "components_spawn");
        serializer.serialize(components_healing_on_change_max_hp, "components_healing_on_change_max_hp");
        serializer.serialize(components_drop_heart, "components_drop_heart");
        serializer.serialize(components_heart, "components_heart");
        serializer.serialize(components_heart_add_stats, "components_heart_add_stats");
        serializer.serialize(components_heart_add_stats_hp, "components_heart_add_stats_hp");
        serializer.serialize(components_heart_add_stats_damage, "components_heart_add_stats_damage");
        serializer.serialize(components_random_healing, "components_random_healing");
        serializer.serialize(components_vampire, "components_vampire");
        serializer.serialize(components_healing_low_hp, "components_healing_low_hp");
        serializer.serialize(components_body, "components_body");
        serializer.serialize(components_sword_cast, "components_sword_cast");
        serializer.serialize(components_sword, "components_sword");
    }

    void ModelEcsBase::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(next_free_id, "next_free_id", int(1));
        deserializer.deserialize(player_id, "player_id", int(0));
        deserializer.deserialize(entities, "entities");
        deserializer.deserialize(game_timer, "game_timer", float(0.0));
        deserializer.deserialize(tasks, "tasks");
        deserializer.deserialize(data, "data");
        deserializer.deserialize(wave_index, "wave_index", int(0));
        deserializer.deserialize(ground, "ground");
        deserializer.deserialize(enemies_level, "enemies_level", int(-1));
        deserializer.deserialize(enemies_rank, "enemies_rank", int(-1));
        deserializer.deserialize(wave_finished, "wave_finished", bool(false));
        deserializer.deserialize(timer_wave_interval, "timer_wave_interval", float(0));
        deserializer.deserialize(timer_wave_duration, "timer_wave_duration", float(0));
        deserializer.deserialize(has_skills, "has_skills");
        deserializer.deserialize(spawn_points, "spawn_points");
        deserializer.deserialize(components_data, "components_data");
        deserializer.deserialize(components_transform, "components_transform");
        deserializer.deserialize(components_stats, "components_stats");
        deserializer.deserialize(components_side, "components_side");
        deserializer.deserialize(components_targetable, "components_targetable");
        deserializer.deserialize(components_health, "components_health");
        deserializer.deserialize(components_damage, "components_damage");
        deserializer.deserialize(components_user, "components_user");
        deserializer.deserialize(components_spine_info, "components_spine_info");
        deserializer.deserialize(components_busy, "components_busy");
        deserializer.deserialize(components_create_units_on_death, "components_create_units_on_death");
        deserializer.deserialize(components_aura, "components_aura");
        deserializer.deserialize(components_aura_of_slow, "components_aura_of_slow");
        deserializer.deserialize(components_aura_damage, "components_aura_damage");
        deserializer.deserialize(components_spirit, "components_spirit");
        deserializer.deserialize(components_spirit_base_points, "components_spirit_base_points");
        deserializer.deserialize(components_spawn_spirit, "components_spawn_spirit");
        deserializer.deserialize(components_recharge, "components_recharge");
        deserializer.deserialize(components_bullet, "components_bullet");
        deserializer.deserialize(components_bullet_laser, "components_bullet_laser");
        deserializer.deserialize(components_shoot_bullet, "components_shoot_bullet");
        deserializer.deserialize(components_melee_attack, "components_melee_attack");
        deserializer.deserialize(components_target_highlight, "components_target_highlight");
        deserializer.deserialize(components_bullet_split, "components_bullet_split");
        deserializer.deserialize(components_shield, "components_shield");
        deserializer.deserialize(components_effects, "components_effects");
        deserializer.deserialize(components_bullet_follow_to_target, "components_bullet_follow_to_target");
        deserializer.deserialize(components_meteor_periodic, "components_meteor_periodic");
        deserializer.deserialize(components_electric_damage, "components_electric_damage");
        deserializer.deserialize(components_sphere_spawn, "components_sphere_spawn");
        deserializer.deserialize(components_sphere, "components_sphere");
        deserializer.deserialize(components_actions, "components_actions");
        deserializer.deserialize(components_gate, "components_gate");
        deserializer.deserialize(components_level_up, "components_level_up");
        deserializer.deserialize(components_exp, "components_exp");
        deserializer.deserialize(components_exp_drop, "components_exp_drop");
        deserializer.deserialize(components_stun, "components_stun");
        deserializer.deserialize(components_fire_damage, "components_fire_damage");
        deserializer.deserialize(components_movement, "components_movement");
        deserializer.deserialize(components_move_direction, "components_move_direction");
        deserializer.deserialize(components_move_to_target, "components_move_to_target");
        deserializer.deserialize(components_move_instant, "components_move_instant");
        deserializer.deserialize(components_move_vertical, "components_move_vertical");
        deserializer.deserialize(components_freezing, "components_freezing");
        deserializer.deserialize(components_move_parabolic, "components_move_parabolic");
        deserializer.deserialize(components_create_movement_to_hero, "components_create_movement_to_hero");
        deserializer.deserialize(components_push, "components_push");
        deserializer.deserialize(components_spawn, "components_spawn");
        deserializer.deserialize(components_healing_on_change_max_hp, "components_healing_on_change_max_hp");
        deserializer.deserialize(components_drop_heart, "components_drop_heart");
        deserializer.deserialize(components_heart, "components_heart");
        deserializer.deserialize(components_heart_add_stats, "components_heart_add_stats");
        deserializer.deserialize(components_heart_add_stats_hp, "components_heart_add_stats_hp");
        deserializer.deserialize(components_heart_add_stats_damage, "components_heart_add_stats_damage");
        deserializer.deserialize(components_random_healing, "components_random_healing");
        deserializer.deserialize(components_vampire, "components_vampire");
        deserializer.deserialize(components_healing_low_hp, "components_healing_low_hp");
        deserializer.deserialize(components_body, "components_body");
        deserializer.deserialize(components_sword_cast, "components_sword_cast");
        deserializer.deserialize(components_sword, "components_sword");
    }

} //namespace mg
