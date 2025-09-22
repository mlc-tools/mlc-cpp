#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentSpirit.h"
#include "ComponentSpiritBasePoints.h"
#include "ModelEcsBase.h"
#include "SystemSpiritMovement.h"
#include "Transform.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemSpiritMovement::TYPE("SystemSpiritMovement");

    SystemSpiritMovement::SystemSpiritMovement()
    : _reference_counter(1)
    {

    }

    SystemSpiritMovement::~SystemSpiritMovement()
    {
    }

    void SystemSpiritMovement::update(ModelEcsBase* model, float dt)
    {
        auto component_base = model->get<ComponentSpiritBasePoints>(model->player_id);
        auto player_transform = model->get<Transform>(model->player_id);
        model->each<ComponentSpirit, Transform>(
        [&](auto& spirit, auto& transform)
        {
            if(!spirit->initialized)
            {
                this->initialize_spirit_points(model, component_base);
            }

            spirit->base_point.rotate(45 * M_PI / 180 * dt);
            auto offset = player_transform->position + spirit->base_point - transform->position;
            auto distance = offset.length();
            if(distance > 10)
            {
                float speed = 50 + distance * spirit->speed_random_t;
                offset /= distance;
                transform->position += offset * dt * speed;
            }
        });
    }

    void SystemSpiritMovement::initialize_spirit_points(ModelEcsBase* model, ComponentSpiritBasePoints* component_base)
    {
        auto size = static_cast<int>(model->components_spirit.size());
        auto points = this->generate_base_points(model, size);
        int i = 0;
        for(auto& spirit : model->components_spirit)
        {
            component_base->points[spirit->id] = points[i];
            if(!spirit->initialized)
            {
                spirit->speed_random_t = random_float() * 0.4f + 0.8f;
                assert(0.8f <= spirit->speed_random_t && spirit->speed_random_t <= 1.2f);
                spirit->initialized = true;
            }
            spirit->base_point = points[i];
            ++i;
        }
    }

    std::vector<Vector> SystemSpiritMovement::generate_base_points(ModelEcsBase* model, int count)
    {
        float base_angle = 270;
        float angle_on_one_spirit = 30;
        float half = (count - 1) / 2.f;
        float start_angle = base_angle - angle_on_one_spirit * half;
        std::vector<Vector> points;
        for(int i=0; i<count; ++i)
        {
            points.push_back(Vector::build_vector(start_angle * M_PI / 180) * 100);
            start_angle += angle_on_one_spirit;
        }
        return points;
    }

    void SystemSpiritMovement::retain()
    {
        ++this->_reference_counter;
    }

    int SystemSpiritMovement::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemSpiritMovement::operator ==(const SystemSpiritMovement& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemSpiritMovement::operator !=(const SystemSpiritMovement& rhs) const
    {
        return !(*this == rhs);
    }

    SystemSpiritMovement::SystemSpiritMovement(const SystemSpiritMovement& rhs)
    {
        this->operator=(rhs);
    }

    const SystemSpiritMovement& SystemSpiritMovement::operator =(const SystemSpiritMovement& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemSpiritMovement::get_type() const
    {
        return SystemSpiritMovement::TYPE;
    }

    void SystemSpiritMovement::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemSpiritMovement::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemSpiritMovement::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemSpiritMovement::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
