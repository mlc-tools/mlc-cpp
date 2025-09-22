#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "BuilderEntity.h"
#include "ComponentBullet.h"
#include "ComponentSphere.h"
#include "ComponentSphereSpawn.h"
#include "ModelEcsBase.h"
#include "Side.h"
#include "SystemSphere.h"
#include "Transform.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemSphere::TYPE("SystemSphere");

    SystemSphere::SystemSphere()
    : _reference_counter(1)
    {

    }

    SystemSphere::~SystemSphere()
    {
    }

    void SystemSphere::update(ModelEcsBase* model, float dt)
    {
        this->check_spawn(model);
        this->update_positions(model, dt);
    }

    void SystemSphere::check_spawn(ModelEcsBase* model)
    {
        auto spawn = model->get<ComponentSphereSpawn>(model->player_id);
        if(spawn && !spawn->spawned)
        {
            this->spawn(model, spawn);
        }
    }

    void SystemSphere::spawn(ModelEcsBase* model, ComponentSphereSpawn* spawn)
    {
        auto hero_transform = model->get<Transform>(model->player_id);
        for(int i=0; i<spawn->count; ++i)
        {
            auto id = BuilderEntity(spawn->data).set_side(Side::ally).set_position(hero_transform->position).build(model);
            auto sphere = model->get<ComponentSphere>(id);
            auto bullet = model->get<ComponentBullet>(id);
            sphere->hero_id = model->player_id;
            bullet->shooter_id = model->player_id;
            bullet->always_life = true;

            model->event_create_bullet.notify(id, hero_transform->position);
        }
        spawn->spawned = true;
        auto count = model->components_sphere.size();
        int i = 0;
        model->each<ComponentSphere>([&](auto& sphere)
        {
            sphere->angle = 2 * M_PI * i++ / count;
        });
    }

    void SystemSphere::update_positions(ModelEcsBase* model, float dt)
    {
        model->each<ComponentSphere, Transform>(
        [&](auto& sphere, auto& transform)
        {
            auto hero_transform = model->get<Transform>(sphere->hero_id);
            sphere->angle += sphere->speed * dt;
            if(sphere->angle > M_PI * 2)
            {
                sphere->angle -= M_PI * 2;
            }
            auto offset = Vector::build_vector(sphere->angle) * sphere->radius;
            offset.y *= 0.8;
            transform->position = hero_transform->position + offset;
            transform->rotation = 360 -sphere->angle * 180 / M_PI + 270;
        });
    }

    void SystemSphere::retain()
    {
        ++this->_reference_counter;
    }

    int SystemSphere::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemSphere::operator ==(const SystemSphere& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemSphere::operator !=(const SystemSphere& rhs) const
    {
        return !(*this == rhs);
    }

    SystemSphere::SystemSphere(const SystemSphere& rhs)
    {
        this->operator=(rhs);
    }

    const SystemSphere& SystemSphere::operator =(const SystemSphere& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemSphere::get_type() const
    {
        return SystemSphere::TYPE;
    }

    void SystemSphere::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemSphere::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemSphere::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemSphere::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
