#ifndef __mg_CollisionChecker_h__
#define __mg_CollisionChecker_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "CollisionResult.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class Circle;
    class Polygon;
    class Ray;
    class Segment;
    class Vector;

    class CollisionChecker
    {
    public:
        CollisionChecker();
        ~CollisionChecker();
        static bool has_collision(const Ray& ray, const Circle& circle);
        static bool has_collision(const Segment& seg, const Circle& circle);
        static bool has_collision(const Ray& ray, const Polygon& polygon, Vector& collision_point);
        static bool is_point_inside_polygon(const Vector& point, const Polygon& polygon);
        static CollisionResult polygon_circle_mtv(const Polygon& polygon, const Circle& circle);
        void retain();
        int release();
        bool operator ==(const CollisionChecker& rhs) const;
        bool operator !=(const CollisionChecker& rhs) const;
        CollisionChecker(const CollisionChecker& rhs);
        const CollisionChecker& operator =(const CollisionChecker& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_CollisionChecker_h__
