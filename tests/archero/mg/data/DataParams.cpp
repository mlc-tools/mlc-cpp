#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../ecs/SkillRarity.h"
#include "../model/System.h"
#include "DataParams.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string DataParams::DEFAULT("default");
    std::string DataParams::SERVER_VERSION_ON_CLIENT("1.0");
    std::string DataParams::SERVER_VERSION_ON_SERVER("1.0");
    const std::string DataParams::TYPE("DataParams");

    DataParams::DataParams()
    : name("")
    , app_package("")
    , crypt_resources(true)
    , features()
    , user_systems()
    , skill_rarity_weights()
    , _reference_counter(1)
    {

    }

    DataParams::~DataParams()
    {
    }

    void DataParams::retain()
    {
        ++this->_reference_counter;
    }

    int DataParams::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool DataParams::operator ==(const DataParams& rhs) const
    {
        bool result = true;
        result = result && this->name == rhs.name;
        result = result && this->app_package == rhs.app_package;
        result = result && this->crypt_resources == rhs.crypt_resources;
        result = result && this->features == rhs.features;
        result = result && this->user_systems == rhs.user_systems;
        result = result && this->skill_rarity_weights == rhs.skill_rarity_weights;
        return result;
    }

    bool DataParams::operator !=(const DataParams& rhs) const
    {
        return !(*this == rhs);
    }

    DataParams::DataParams(const DataParams& rhs)
    {
        this->operator=(rhs);
    }

    const DataParams& DataParams::operator =(const DataParams& rhs)
    {
        this->name = rhs.name;
        this->app_package = rhs.app_package;
        this->crypt_resources = rhs.crypt_resources;
        this->features = rhs.features;
        this->user_systems = rhs.user_systems;
        this->skill_rarity_weights = rhs.skill_rarity_weights;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string DataParams::get_type() const
    {
        return DataParams::TYPE;
    }

    void DataParams::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(app_package, "app_package", std::string(""));
        serializer.serialize(crypt_resources, "crypt_resources", bool(true));
        serializer.serialize(features, "features");
        serializer.serialize(user_systems, "user_systems");
        serializer.serialize(skill_rarity_weights, "skill_rarity_weights");
    }

    void DataParams::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(app_package, "app_package", std::string(""));
        deserializer.deserialize(crypt_resources, "crypt_resources", bool(true));
        deserializer.deserialize(features, "features");
        deserializer.deserialize(user_systems, "user_systems");
        deserializer.deserialize(skill_rarity_weights, "skill_rarity_weights");
    }

    void DataParams::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(name, "name", std::string(""));
        serializer.serialize(app_package, "app_package", std::string(""));
        serializer.serialize(crypt_resources, "crypt_resources", bool(true));
        serializer.serialize(features, "features");
        serializer.serialize(user_systems, "user_systems");
        serializer.serialize(skill_rarity_weights, "skill_rarity_weights");
    }

    void DataParams::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(name, "name", std::string(""));
        deserializer.deserialize(app_package, "app_package", std::string(""));
        deserializer.deserialize(crypt_resources, "crypt_resources", bool(true));
        deserializer.deserialize(features, "features");
        deserializer.deserialize(user_systems, "user_systems");
        deserializer.deserialize(skill_rarity_weights, "skill_rarity_weights");
    }

} //namespace mg
