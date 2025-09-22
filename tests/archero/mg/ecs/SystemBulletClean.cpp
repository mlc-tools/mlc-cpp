#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentBullet.h"
#include "ComponentBulletLaser.h"
#include "ModelEcsBase.h"
#include "SystemBullet.h"
#include "SystemBulletClean.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemBulletClean::TYPE("SystemBulletClean");

    SystemBulletClean::SystemBulletClean()
    : _reference_counter(1)
    {

    }

    SystemBulletClean::~SystemBulletClean()
    {
    }

    void SystemBulletClean::update(ModelEcsBase* model, float dt)
    {

    }

    void SystemBulletClean::clean(ModelEcsBase* model)
    {

        for(int __index__ = 0; __index__ < model->components_bullet.size(); ++__index__)
        {
            auto& component = model->components_bullet.at(__index__);
            if(component->clean )
            {
                auto __size__ = model->components_bullet.size();
                SystemBullet::remove_bullet(model, component);
                if(__size__ != model->components_bullet.size())
                {
                    __index__ -= 1;
                }
            }
        }

        for(int __index__ = 0; __index__ < model->components_bullet_laser.size(); ++__index__)
        {
            auto& bullet = model->components_bullet_laser.at(__index__);
            if(bullet->duration < 0 )
            {
                auto __size__ = model->components_bullet_laser.size();
                SystemBullet::remove_bullet(model, bullet);
                if(__size__ != model->components_bullet_laser.size())
                {
                    __index__ -= 1;
                }
            }
        }
    }

    void SystemBulletClean::retain()
    {
        ++this->_reference_counter;
    }

    int SystemBulletClean::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemBulletClean::operator ==(const SystemBulletClean& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemBulletClean::operator !=(const SystemBulletClean& rhs) const
    {
        return !(*this == rhs);
    }

    SystemBulletClean::SystemBulletClean(const SystemBulletClean& rhs)
    {
        this->operator=(rhs);
    }

    const SystemBulletClean& SystemBulletClean::operator =(const SystemBulletClean& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemBulletClean::get_type() const
    {
        return SystemBulletClean::TYPE;
    }

    void SystemBulletClean::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemBulletClean::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemBulletClean::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemBulletClean::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
