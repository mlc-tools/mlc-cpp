#include "Registrar.h"
#include "mg_Factory.h"
#include "DataStorage.h"
#include "VisualItem.h"
#include "data/DataEquipment.h"
#include "data/DataEquipmentStat.h"
#include "data/DataLadderLevels.h"
#include "data/DataLocale.h"
#include "data/DataMergeLevel.h"
#include "data/DataParams.h"
#include "ecs/ActionBase.h"
#include "ecs/ActionCreateDamage.h"
#include "ecs/ActionCreateMarkerNearHero.h"
#include "ecs/ActionHorizontalOrVerticalOrDiagonal.h"
#include "ecs/ActionMeleeAttack.h"
#include "ecs/ActionMoveDirection.h"
#include "ecs/ActionMoveToCenterHorizontalOrVertical.h"
#include "ecs/ActionMoveToPlayer.h"
#include "ecs/ActionRunToPlayer.h"
#include "ecs/ActionShoot.h"
#include "ecs/ActionShoot4Dir.h"
#include "ecs/ActionShootByMoveDirection.h"
#include "ecs/ActionWait.h"
#include "ecs/BBox.h"
#include "ecs/BuilderBullet.h"
#include "ecs/BuilderDamage.h"
#include "ecs/BuilderEntity.h"
#include "ecs/Cell.h"
#include "ecs/Circle.h"
#include "ecs/CollisionChecker.h"
#include "ecs/CollisionResult.h"
#include "ecs/ComponentActions.h"
#include "ecs/ComponentAura.h"
#include "ecs/ComponentAuraDamage.h"
#include "ecs/ComponentAuraOfSlow.h"
#include "ecs/ComponentBody.h"
#include "ecs/ComponentBullet.h"
#include "ecs/ComponentBulletFollowToTarget.h"
#include "ecs/ComponentBulletLaser.h"
#include "ecs/ComponentBulletSplit.h"
#include "ecs/ComponentBusy.h"
#include "ecs/ComponentCreateMovementToHero.h"
#include "ecs/ComponentCreateUnitsOnDeath.h"
#include "ecs/ComponentDamage.h"
#include "ecs/ComponentData.h"
#include "ecs/ComponentDropHeart.h"
#include "ecs/ComponentEffects.h"
#include "ecs/ComponentExp.h"
#include "ecs/ComponentExpDrop.h"
#include "ecs/ComponentGate.h"
#include "ecs/ComponentHealingLowHp.h"
#include "ecs/ComponentHealingOnChangeMaxHp.h"
#include "ecs/ComponentHealth.h"
#include "ecs/ComponentHeart.h"
#include "ecs/ComponentHeartAddStats.h"
#include "ecs/ComponentHeartAddStatsDamage.h"
#include "ecs/ComponentHeartAddStatsHp.h"
#include "ecs/ComponentLevelUp.h"
#include "ecs/ComponentMeleeAttack.h"
#include "ecs/ComponentMeteorPeriodic.h"
#include "ecs/ComponentMovement.h"
#include "ecs/ComponentPush.h"
#include "ecs/ComponentRandomHealing.h"
#include "ecs/ComponentRecharge.h"
#include "ecs/ComponentShield.h"
#include "ecs/ComponentShootBullet.h"
#include "ecs/ComponentSide.h"
#include "ecs/ComponentSpawn.h"
#include "ecs/ComponentSpawnSpirit.h"
#include "ecs/ComponentSphere.h"
#include "ecs/ComponentSphereSpawn.h"
#include "ecs/ComponentSpirit.h"
#include "ecs/ComponentSpiritBasePoints.h"
#include "ecs/ComponentStats.h"
#include "ecs/ComponentStun.h"
#include "ecs/ComponentSword.h"
#include "ecs/ComponentSwordCast.h"
#include "ecs/ComponentTargetHighlight.h"
#include "ecs/ComponentTargetable.h"
#include "ecs/ComponentUser.h"
#include "ecs/ComponentVampire.h"
#include "ecs/ControllerDungeonBase.h"
#include "ecs/DataLevel.h"
#include "ecs/DataStatUpgrade.h"
#include "ecs/DataStatUpgradeVisual.h"
#include "ecs/DataUnit.h"
#include "ecs/DataUnitVisual.h"
#include "ecs/DataWaveOnRadius.h"
#include "ecs/DataWaveOnRadiusWithInterval.h"
#include "ecs/DataWaveOnSpawnPoints.h"
#include "ecs/DataWaveOnSpawnPointsRandom.h"
#include "ecs/Dictionary.h"
#include "ecs/ElectricDamage.h"
#include "ecs/EntityBase.h"
#include "ecs/FireDamage.h"
#include "ecs/Freezing.h"
#include "ecs/Ground.h"
#include "ecs/Math.h"
#include "ecs/ModelEcsBase.h"
#include "ecs/Modifier.h"
#include "ecs/MoveDirection.h"
#include "ecs/MoveInstant.h"
#include "ecs/MoveParabolic.h"
#include "ecs/MoveToTarget.h"
#include "ecs/MoveVertical.h"
#include "ecs/PhysicalBody.h"
#include "ecs/Polygon.h"
#include "ecs/Ray.h"
#include "ecs/Segment.h"
#include "ecs/SpawnInfo.h"
#include "ecs/SystemActions.h"
#include "ecs/SystemAuraDamage.h"
#include "ecs/SystemAuroOfSlow.h"
#include "ecs/SystemBullet.h"
#include "ecs/SystemBulletClean.h"
#include "ecs/SystemBulletFollowToTarget.h"
#include "ecs/SystemCreateMovement.h"
#include "ecs/SystemCreateUnitsOnDeath.h"
#include "ecs/SystemDamage.h"
#include "ecs/SystemDeath.h"
#include "ecs/SystemDropHeart.h"
#include "ecs/SystemEffects.h"
#include "ecs/SystemElectricDamage.h"
#include "ecs/SystemExp.h"
#include "ecs/SystemFireDamage.h"
#include "ecs/SystemFreezing.h"
#include "ecs/SystemGate.h"
#include "ecs/SystemHealingOnChangeMaxHp.h"
#include "ecs/SystemMeleeAttack.h"
#include "ecs/SystemMeteor.h"
#include "ecs/SystemMovement.h"
#include "ecs/SystemPush.h"
#include "ecs/SystemRemoveDamage.h"
#include "ecs/SystemResolveCollisions.h"
#include "ecs/SystemShieldRestore.h"
#include "ecs/SystemShoot.h"
#include "ecs/SystemSpawn.h"
#include "ecs/SystemSphere.h"
#include "ecs/SystemSpiritMovement.h"
#include "ecs/SystemSpiritSpawn.h"
#include "ecs/SystemStats.h"
#include "ecs/SystemStun.h"
#include "ecs/SystemSwords.h"
#include "ecs/SystemSwordsCast.h"
#include "ecs/SystemUpgrade.h"
#include "ecs/SystemVampire.h"
#include "ecs/Task.h"
#include "ecs/Transform.h"
#include "ecs/Vector.h"
#include "inet/CommandBase.h"
#include "inet/CommandSequence.h"
#include "inet/Request.h"
#include "inet/Response.h"
#include "inet/ResponseError.h"
#include "inet/ResponseOk.h"
#include "model/Model.h"
#include "model/ModelEquipment.h"
#include "model/ModelEquipmentSlot.h"
#include "model/ModelUser.h"
#include "model/ModelUserBase.h"
#include "model/UserProfile.h"
#include "systems/SystemEquipment.h"
#include "utils/Shuffle.h"

void mg::register_classes()
{
    Factory::shared().registrationCommand<DataStorage>(DataStorage::TYPE);
    Factory::shared().registrationCommand<VisualItem>(VisualItem::TYPE);
    Factory::shared().registrationCommand<DataEquipment>(DataEquipment::TYPE);
    Factory::shared().registrationCommand<DataEquipmentStat>(DataEquipmentStat::TYPE);
    Factory::shared().registrationCommand<DataLadderLevels>(DataLadderLevels::TYPE);
    Factory::shared().registrationCommand<DataLocale>(DataLocale::TYPE);
    Factory::shared().registrationCommand<DataMergeLevel>(DataMergeLevel::TYPE);
    Factory::shared().registrationCommand<DataParams>(DataParams::TYPE);
    Factory::shared().registrationCommand<ActionBase>(ActionBase::TYPE);
    Factory::shared().registrationCommand<ActionCreateDamage>(ActionCreateDamage::TYPE);
    Factory::shared().registrationCommand<ActionCreateMarkerNearHero>(ActionCreateMarkerNearHero::TYPE);
    Factory::shared().registrationCommand<ActionHorizontalOrVerticalOrDiagonal>(ActionHorizontalOrVerticalOrDiagonal::TYPE);
    Factory::shared().registrationCommand<ActionMeleeAttack>(ActionMeleeAttack::TYPE);
    Factory::shared().registrationCommand<ActionMoveDirection>(ActionMoveDirection::TYPE);
    Factory::shared().registrationCommand<ActionMoveToCenterHorizontalOrVertical>(ActionMoveToCenterHorizontalOrVertical::TYPE);
    Factory::shared().registrationCommand<ActionMoveToPlayer>(ActionMoveToPlayer::TYPE);
    Factory::shared().registrationCommand<ActionRunToPlayer>(ActionRunToPlayer::TYPE);
    Factory::shared().registrationCommand<ActionShoot>(ActionShoot::TYPE);
    Factory::shared().registrationCommand<ActionShoot4Dir>(ActionShoot4Dir::TYPE);
    Factory::shared().registrationCommand<ActionShootByMoveDirection>(ActionShootByMoveDirection::TYPE);
    Factory::shared().registrationCommand<ActionWait>(ActionWait::TYPE);
    Factory::shared().registrationCommand<BBox>(BBox::TYPE);
    Factory::shared().registrationCommand<BuilderBullet>(BuilderBullet::TYPE);
    Factory::shared().registrationCommand<BuilderDamage>(BuilderDamage::TYPE);
    Factory::shared().registrationCommand<BuilderEntity>(BuilderEntity::TYPE);
    Factory::shared().registrationCommand<Cell>(Cell::TYPE);
    Factory::shared().registrationCommand<Circle>(Circle::TYPE);
    Factory::shared().registrationCommand<CollisionChecker>(CollisionChecker::TYPE);
    Factory::shared().registrationCommand<CollisionResult>(CollisionResult::TYPE);
    Factory::shared().registrationCommand<ComponentActions>(ComponentActions::TYPE);
    Factory::shared().registrationCommand<ComponentAura>(ComponentAura::TYPE);
    Factory::shared().registrationCommand<ComponentAuraDamage>(ComponentAuraDamage::TYPE);
    Factory::shared().registrationCommand<ComponentAuraOfSlow>(ComponentAuraOfSlow::TYPE);
    Factory::shared().registrationCommand<ComponentBody>(ComponentBody::TYPE);
    Factory::shared().registrationCommand<ComponentBullet>(ComponentBullet::TYPE);
    Factory::shared().registrationCommand<ComponentBulletFollowToTarget>(ComponentBulletFollowToTarget::TYPE);
    Factory::shared().registrationCommand<ComponentBulletLaser>(ComponentBulletLaser::TYPE);
    Factory::shared().registrationCommand<ComponentBulletSplit>(ComponentBulletSplit::TYPE);
    Factory::shared().registrationCommand<ComponentBusy>(ComponentBusy::TYPE);
    Factory::shared().registrationCommand<ComponentCreateMovementToHero>(ComponentCreateMovementToHero::TYPE);
    Factory::shared().registrationCommand<ComponentCreateUnitsOnDeath>(ComponentCreateUnitsOnDeath::TYPE);
    Factory::shared().registrationCommand<ComponentDamage>(ComponentDamage::TYPE);
    Factory::shared().registrationCommand<ComponentData>(ComponentData::TYPE);
    Factory::shared().registrationCommand<ComponentDropHeart>(ComponentDropHeart::TYPE);
    Factory::shared().registrationCommand<ComponentEffects>(ComponentEffects::TYPE);
    Factory::shared().registrationCommand<ComponentExp>(ComponentExp::TYPE);
    Factory::shared().registrationCommand<ComponentExpDrop>(ComponentExpDrop::TYPE);
    Factory::shared().registrationCommand<ComponentGate>(ComponentGate::TYPE);
    Factory::shared().registrationCommand<ComponentHealingLowHp>(ComponentHealingLowHp::TYPE);
    Factory::shared().registrationCommand<ComponentHealingOnChangeMaxHp>(ComponentHealingOnChangeMaxHp::TYPE);
    Factory::shared().registrationCommand<ComponentHealth>(ComponentHealth::TYPE);
    Factory::shared().registrationCommand<ComponentHeart>(ComponentHeart::TYPE);
    Factory::shared().registrationCommand<ComponentHeartAddStats>(ComponentHeartAddStats::TYPE);
    Factory::shared().registrationCommand<ComponentHeartAddStatsDamage>(ComponentHeartAddStatsDamage::TYPE);
    Factory::shared().registrationCommand<ComponentHeartAddStatsHp>(ComponentHeartAddStatsHp::TYPE);
    Factory::shared().registrationCommand<ComponentLevelUp>(ComponentLevelUp::TYPE);
    Factory::shared().registrationCommand<ComponentMeleeAttack>(ComponentMeleeAttack::TYPE);
    Factory::shared().registrationCommand<ComponentMeteorPeriodic>(ComponentMeteorPeriodic::TYPE);
    Factory::shared().registrationCommand<ComponentMovement>(ComponentMovement::TYPE);
    Factory::shared().registrationCommand<ComponentPush>(ComponentPush::TYPE);
    Factory::shared().registrationCommand<ComponentRandomHealing>(ComponentRandomHealing::TYPE);
    Factory::shared().registrationCommand<ComponentRecharge>(ComponentRecharge::TYPE);
    Factory::shared().registrationCommand<ComponentShield>(ComponentShield::TYPE);
    Factory::shared().registrationCommand<ComponentShootBullet>(ComponentShootBullet::TYPE);
    Factory::shared().registrationCommand<ComponentSide>(ComponentSide::TYPE);
    Factory::shared().registrationCommand<ComponentSpawn>(ComponentSpawn::TYPE);
    Factory::shared().registrationCommand<ComponentSpawnSpirit>(ComponentSpawnSpirit::TYPE);
    Factory::shared().registrationCommand<ComponentSphere>(ComponentSphere::TYPE);
    Factory::shared().registrationCommand<ComponentSphereSpawn>(ComponentSphereSpawn::TYPE);
    Factory::shared().registrationCommand<ComponentSpirit>(ComponentSpirit::TYPE);
    Factory::shared().registrationCommand<ComponentSpiritBasePoints>(ComponentSpiritBasePoints::TYPE);
    Factory::shared().registrationCommand<ComponentStats>(ComponentStats::TYPE);
    Factory::shared().registrationCommand<ComponentStun>(ComponentStun::TYPE);
    Factory::shared().registrationCommand<ComponentSword>(ComponentSword::TYPE);
    Factory::shared().registrationCommand<ComponentSwordCast>(ComponentSwordCast::TYPE);
    Factory::shared().registrationCommand<ComponentTargetHighlight>(ComponentTargetHighlight::TYPE);
    Factory::shared().registrationCommand<ComponentTargetable>(ComponentTargetable::TYPE);
    Factory::shared().registrationCommand<ComponentUser>(ComponentUser::TYPE);
    Factory::shared().registrationCommand<ComponentVampire>(ComponentVampire::TYPE);
    Factory::shared().registrationCommand<ControllerDungeonBase>(ControllerDungeonBase::TYPE);
    Factory::shared().registrationCommand<DataLevel>(DataLevel::TYPE);
    Factory::shared().registrationCommand<DataStatUpgrade>(DataStatUpgrade::TYPE);
    Factory::shared().registrationCommand<DataStatUpgradeVisual>(DataStatUpgradeVisual::TYPE);
    Factory::shared().registrationCommand<DataUnit>(DataUnit::TYPE);
    Factory::shared().registrationCommand<DataUnitVisual>(DataUnitVisual::TYPE);
    Factory::shared().registrationCommand<DataWaveOnRadius>(DataWaveOnRadius::TYPE);
    Factory::shared().registrationCommand<DataWaveOnRadiusWithInterval>(DataWaveOnRadiusWithInterval::TYPE);
    Factory::shared().registrationCommand<DataWaveOnSpawnPoints>(DataWaveOnSpawnPoints::TYPE);
    Factory::shared().registrationCommand<DataWaveOnSpawnPointsRandom>(DataWaveOnSpawnPointsRandom::TYPE);
    Factory::shared().registrationCommand<Dictionary>(Dictionary::TYPE);
    Factory::shared().registrationCommand<ElectricDamage>(ElectricDamage::TYPE);
    Factory::shared().registrationCommand<EntityBase>(EntityBase::TYPE);
    Factory::shared().registrationCommand<FireDamage>(FireDamage::TYPE);
    Factory::shared().registrationCommand<Freezing>(Freezing::TYPE);
    Factory::shared().registrationCommand<Ground>(Ground::TYPE);
    Factory::shared().registrationCommand<Math>(Math::TYPE);
    Factory::shared().registrationCommand<ModelEcsBase>(ModelEcsBase::TYPE);
    Factory::shared().registrationCommand<Modifier>(Modifier::TYPE);
    Factory::shared().registrationCommand<MoveDirection>(MoveDirection::TYPE);
    Factory::shared().registrationCommand<MoveInstant>(MoveInstant::TYPE);
    Factory::shared().registrationCommand<MoveParabolic>(MoveParabolic::TYPE);
    Factory::shared().registrationCommand<MoveToTarget>(MoveToTarget::TYPE);
    Factory::shared().registrationCommand<MoveVertical>(MoveVertical::TYPE);
    Factory::shared().registrationCommand<PhysicalBody>(PhysicalBody::TYPE);
    Factory::shared().registrationCommand<Polygon>(Polygon::TYPE);
    Factory::shared().registrationCommand<Ray>(Ray::TYPE);
    Factory::shared().registrationCommand<Segment>(Segment::TYPE);
    Factory::shared().registrationCommand<SpawnInfo>(SpawnInfo::TYPE);
    Factory::shared().registrationCommand<SystemActions>(SystemActions::TYPE);
    Factory::shared().registrationCommand<SystemAuraDamage>(SystemAuraDamage::TYPE);
    Factory::shared().registrationCommand<SystemAuroOfSlow>(SystemAuroOfSlow::TYPE);
    Factory::shared().registrationCommand<SystemBullet>(SystemBullet::TYPE);
    Factory::shared().registrationCommand<SystemBulletClean>(SystemBulletClean::TYPE);
    Factory::shared().registrationCommand<SystemBulletFollowToTarget>(SystemBulletFollowToTarget::TYPE);
    Factory::shared().registrationCommand<SystemCreateMovement>(SystemCreateMovement::TYPE);
    Factory::shared().registrationCommand<SystemCreateUnitsOnDeath>(SystemCreateUnitsOnDeath::TYPE);
    Factory::shared().registrationCommand<SystemDamage>(SystemDamage::TYPE);
    Factory::shared().registrationCommand<SystemDeath>(SystemDeath::TYPE);
    Factory::shared().registrationCommand<SystemDropHeart>(SystemDropHeart::TYPE);
    Factory::shared().registrationCommand<SystemEffects>(SystemEffects::TYPE);
    Factory::shared().registrationCommand<SystemElectricDamage>(SystemElectricDamage::TYPE);
    Factory::shared().registrationCommand<SystemExp>(SystemExp::TYPE);
    Factory::shared().registrationCommand<SystemFireDamage>(SystemFireDamage::TYPE);
    Factory::shared().registrationCommand<SystemFreezing>(SystemFreezing::TYPE);
    Factory::shared().registrationCommand<SystemGate>(SystemGate::TYPE);
    Factory::shared().registrationCommand<SystemHealingOnChangeMaxHp>(SystemHealingOnChangeMaxHp::TYPE);
    Factory::shared().registrationCommand<SystemMeleeAttack>(SystemMeleeAttack::TYPE);
    Factory::shared().registrationCommand<SystemMeteor>(SystemMeteor::TYPE);
    Factory::shared().registrationCommand<SystemMovement>(SystemMovement::TYPE);
    Factory::shared().registrationCommand<SystemPush>(SystemPush::TYPE);
    Factory::shared().registrationCommand<SystemRemoveDamage>(SystemRemoveDamage::TYPE);
    Factory::shared().registrationCommand<SystemResolveCollisions>(SystemResolveCollisions::TYPE);
    Factory::shared().registrationCommand<SystemShieldRestore>(SystemShieldRestore::TYPE);
    Factory::shared().registrationCommand<SystemShoot>(SystemShoot::TYPE);
    Factory::shared().registrationCommand<SystemSpawn>(SystemSpawn::TYPE);
    Factory::shared().registrationCommand<SystemSphere>(SystemSphere::TYPE);
    Factory::shared().registrationCommand<SystemSpiritMovement>(SystemSpiritMovement::TYPE);
    Factory::shared().registrationCommand<SystemSpiritSpawn>(SystemSpiritSpawn::TYPE);
    Factory::shared().registrationCommand<SystemStats>(SystemStats::TYPE);
    Factory::shared().registrationCommand<SystemStun>(SystemStun::TYPE);
    Factory::shared().registrationCommand<SystemSwords>(SystemSwords::TYPE);
    Factory::shared().registrationCommand<SystemSwordsCast>(SystemSwordsCast::TYPE);
    Factory::shared().registrationCommand<SystemUpgrade>(SystemUpgrade::TYPE);
    Factory::shared().registrationCommand<SystemVampire>(SystemVampire::TYPE);
    Factory::shared().registrationCommand<Task>(Task::TYPE);
    Factory::shared().registrationCommand<Transform>(Transform::TYPE);
    Factory::shared().registrationCommand<Vector>(Vector::TYPE);
    Factory::shared().registrationCommand<CommandBase>(CommandBase::TYPE);
    Factory::shared().registrationCommand<CommandSequence>(CommandSequence::TYPE);
    Factory::shared().registrationCommand<Request>(Request::TYPE);
    Factory::shared().registrationCommand<Response>(Response::TYPE);
    Factory::shared().registrationCommand<ResponseError>(ResponseError::TYPE);
    Factory::shared().registrationCommand<ResponseOk>(ResponseOk::TYPE);
    Factory::shared().registrationCommand<Model>(Model::TYPE);
    Factory::shared().registrationCommand<ModelEquipment>(ModelEquipment::TYPE);
    Factory::shared().registrationCommand<ModelEquipmentSlot>(ModelEquipmentSlot::TYPE);
    Factory::shared().registrationCommand<ModelUser>(ModelUser::TYPE);
    Factory::shared().registrationCommand<ModelUserBase>(ModelUserBase::TYPE);
    Factory::shared().registrationCommand<UserProfile>(UserProfile::TYPE);
    Factory::shared().registrationCommand<SystemEquipment>(SystemEquipment::TYPE);
    Factory::shared().registrationCommand<Shuffle>(Shuffle::TYPE);

}
