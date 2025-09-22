#ifndef __mg_CollisionResult_h__
#define __mg_CollisionResult_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class CollisionResult
    {
    public:
        CollisionResult();
        ~CollisionResult();
        void retain();
        int release();
        bool operator ==(const CollisionResult& rhs) const;
        bool operator !=(const CollisionResult& rhs) const;
        CollisionResult(const CollisionResult& rhs);
        const CollisionResult& operator =(const CollisionResult& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        bool collision;
        Vector mtv;
        float penetration;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_CollisionResult_h__
