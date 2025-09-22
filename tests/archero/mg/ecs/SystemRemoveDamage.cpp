#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "ComponentDamage.h"
#include "ModelEcsBase.h"
#include "SystemRemoveDamage.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string SystemRemoveDamage::TYPE("SystemRemoveDamage");

    SystemRemoveDamage::SystemRemoveDamage()
    : _reference_counter(1)
    {

    }

    SystemRemoveDamage::~SystemRemoveDamage()
    {
    }

    void SystemRemoveDamage::update(ModelEcsBase* model, float dt)
    {

    }

    void SystemRemoveDamage::clean(ModelEcsBase* model)
    {
        model->components_damage.clear();
    }

    void SystemRemoveDamage::retain()
    {
        ++this->_reference_counter;
    }

    int SystemRemoveDamage::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool SystemRemoveDamage::operator ==(const SystemRemoveDamage& rhs) const
    {
        bool result = true;
        return result;
    }

    bool SystemRemoveDamage::operator !=(const SystemRemoveDamage& rhs) const
    {
        return !(*this == rhs);
    }

    SystemRemoveDamage::SystemRemoveDamage(const SystemRemoveDamage& rhs)
    {
        this->operator=(rhs);
    }

    const SystemRemoveDamage& SystemRemoveDamage::operator =(const SystemRemoveDamage& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string SystemRemoveDamage::get_type() const
    {
        return SystemRemoveDamage::TYPE;
    }

    void SystemRemoveDamage::serialize_xml(SerializerXml& serializer) const
    {

    }

    void SystemRemoveDamage::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void SystemRemoveDamage::serialize_json(SerializerJson& serializer) const
    {

    }

    void SystemRemoveDamage::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
