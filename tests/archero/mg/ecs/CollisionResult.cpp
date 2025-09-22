#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "CollisionResult.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string CollisionResult::TYPE("CollisionResult");

    CollisionResult::CollisionResult()
    : collision(false)
    , mtv()
    , penetration(0.f)
    , _reference_counter(1)
    {

    }

    CollisionResult::~CollisionResult()
    {
    }

    void CollisionResult::retain()
    {
        ++this->_reference_counter;
    }

    int CollisionResult::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool CollisionResult::operator ==(const CollisionResult& rhs) const
    {
        bool result = true;
        result = result && this->collision == rhs.collision;
        result = result && this->mtv == rhs.mtv;
        result = result && this->penetration == rhs.penetration;
        return result;
    }

    bool CollisionResult::operator !=(const CollisionResult& rhs) const
    {
        return !(*this == rhs);
    }

    CollisionResult::CollisionResult(const CollisionResult& rhs)
    {
        this->operator=(rhs);
    }

    const CollisionResult& CollisionResult::operator =(const CollisionResult& rhs)
    {
        this->collision = rhs.collision;
        this->mtv = rhs.mtv;
        this->penetration = rhs.penetration;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string CollisionResult::get_type() const
    {
        return CollisionResult::TYPE;
    }

    void CollisionResult::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(collision, "collision", bool(false));
        serializer.serialize(mtv, "mtv");
        serializer.serialize(penetration, "penetration", float(0.f));
    }

    void CollisionResult::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(collision, "collision", bool(false));
        deserializer.deserialize(mtv, "mtv");
        deserializer.deserialize(penetration, "penetration", float(0.f));
    }

    void CollisionResult::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(collision, "collision", bool(false));
        serializer.serialize(mtv, "mtv");
        serializer.serialize(penetration, "penetration", float(0.f));
    }

    void CollisionResult::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(collision, "collision", bool(false));
        deserializer.deserialize(mtv, "mtv");
        deserializer.deserialize(penetration, "penetration", float(0.f));
    }

} //namespace mg
