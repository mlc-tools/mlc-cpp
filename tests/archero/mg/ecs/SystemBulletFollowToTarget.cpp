#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBullet.h"
#include "ComponentBulletFollowToTarget.h"
#include "ComponentTargetHighlight.h"
#include "ModelEcsBase.h"
#include "MoveDirection.h"
#include "SystemBulletFollowToTarget.h"
#include "Transform.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemBulletFollowToTarget::TYPE("SystemBulletFollowToTarget");

    SystemBulletFollowToTarget::SystemBulletFollowToTarget()
    : _reference_counter(1)
    {

    }

    SystemBulletFollowToTarget::~SystemBulletFollowToTarget()
    {
    }

    void SystemBulletFollowToTarget::update(ModelEcsBase* model, float dt)
    {
        model->each_if<ComponentBulletFollowToTarget, ComponentBullet, Transform>(
        [&](auto& follow, auto& bullet, auto& transform)
        {
            return bullet->shooter_id == model->player_id && model->get<Transform>(follow->target_id) == nullptr;
        },
        [&](auto& follow, auto& bullet, auto& transform)
        {
            auto hero_target = model->get<ComponentTargetHighlight>(model->player_id);
            follow->target_id = hero_target->target_id;
        });
        model->each_if<ComponentBulletFollowToTarget, MoveDirection, Transform>(
        [&](auto& follow, auto& movement, auto& transform)
        {
            follow->idle -= dt;
            return follow->idle <= 0 && model->get<Transform>(follow->target_id) != nullptr;
        },
        [&](auto& follow, auto& movement, auto& transform)
        {
            auto target_transform = model->get<Transform>(follow->target_id);
            auto current_angle = Vector::get_angle(movement->direction);

            auto goal_direction = Vector::get_angle(target_transform->position - transform->position);
            auto diff = Vector::get_signed_angle_between(movement->direction, target_transform->position - transform->position);
            if(std::fabs(diff) < 60 * M_PI / 180)
            {
                auto t = std::min(1.f, follow->change_angle_speed * dt);
                auto new_angle = current_angle + diff * t;
                movement->direction = Vector::build_vector(new_angle);
            }
        });
    }

    void SystemBulletFollowToTarget::retain()
    {
        ++this->_reference_counter;
    }

    int SystemBulletFollowToTarget::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemBulletFollowToTarget::operator ==(const SystemBulletFollowToTarget& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemBulletFollowToTarget::operator !=(const SystemBulletFollowToTarget& rhs) const
    {
        return !(*this == rhs);
    }

    SystemBulletFollowToTarget::SystemBulletFollowToTarget(const SystemBulletFollowToTarget& rhs)
    {
        this->operator=(rhs);
    }

    const SystemBulletFollowToTarget& SystemBulletFollowToTarget::operator =(const SystemBulletFollowToTarget& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemBulletFollowToTarget::get_type() const
    {
        return SystemBulletFollowToTarget::TYPE;
    }

    void SystemBulletFollowToTarget::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemBulletFollowToTarget::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemBulletFollowToTarget::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemBulletFollowToTarget::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
