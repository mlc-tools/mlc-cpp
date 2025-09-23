#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Circle.h"
#include "CollisionChecker.h"
#include "CollisionResult.h"
#include "Polygon.h"
#include "Ray.h"
#include "Segment.h"
#include "Vector.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string CollisionChecker::TYPE("CollisionChecker");

    CollisionChecker::CollisionChecker()
    : _reference_counter(1)
    {

    }

    CollisionChecker::~CollisionChecker()
    {
    }

    bool CollisionChecker::has_collision(const Ray& ray, const Circle& circle)
    {
        auto len = ray.direction.length();
        assert(fabs(len - 1) < 0.01f);

        auto m = ray.origin - circle.center;
        float b = m.dot(ray.direction);
        float c = m.dot(m) - circle.radius * circle.radius;

        if (c > 0.0f && b > 0.0f)
        {
            return false;
        }

        float disc = b * b - c;
        if (disc < 0.0f)
        {
            return false;
        }

        float tEnter = -b - sqrtf(std::max(0.0f, disc));
        if (c <= 0.0f)
        {
            tEnter = 0.0f;
        }

        return (tEnter >= 0.0f && tEnter <= std::numeric_limits<float>::infinity());
    }

    bool CollisionChecker::has_collision(const Segment& seg, const Circle& circle)
    {
        Vector d = seg.end - seg.start;
        float len_sq = d.dot(d);
        if (len_sq < Vector::FLOAT_SMALL)
        {
            return seg.start.get_distance_sq(circle.center) <= circle.radius * circle.radius;
        }

        float t = (circle.center - seg.start).dot(d) / len_sq;
        if (t < 0.0f)
        {
            t = 0.0f;
        }
        else if (t > 1.0f)
        {
            t = 1.0f;
        }

        Vector closest = seg.start + d * t;
        return closest.get_distance_sq(circle.center) <= circle.radius * circle.radius;
    }

    bool CollisionChecker::has_collision(const Ray& ray, const Polygon& polygon, Vector& collision_point)
    {
        auto len = ray.direction.length();
        assert(fabs(len - 1) < 0.01f);

        if (polygon.points.size() < 3)
        {
            return false;
        }

        {
            float tmin = -std::numeric_limits<float>::infinity();
            float tmax = std::numeric_limits<float>::infinity();

            if (std::fabs(ray.direction.x) > Vector::FLOAT_SMALL)
            {
                float tx1 = (polygon.bbox.min.x - ray.origin.x) / ray.direction.x;
                float tx2 = (polygon.bbox.max.x - ray.origin.x) / ray.direction.x;
                tmin = std::max(tmin, std::min(tx1, tx2));
                tmax = std::min(tmax, std::max(tx1, tx2));
            }
            else if (ray.origin.x < polygon.bbox.min.x || ray.origin.x > polygon.bbox.max.x)
            {
                return false;
            }

            if (std::fabs(ray.direction.y) > Vector::FLOAT_SMALL)
            {
                float ty1 = (polygon.bbox.min.y - ray.origin.y) / ray.direction.y;
                float ty2 = (polygon.bbox.max.y - ray.origin.y) / ray.direction.y;
                tmin = std::max(tmin, std::min(ty1, ty2));
                tmax = std::min(tmax, std::max(ty1, ty2));
            }
            else if (ray.origin.y < polygon.bbox.min.y || ray.origin.y > polygon.bbox.max.y)
            {
                return false;
            }

            if (tmax < tmin || tmax < 0)
            {
                return false;
            }
        }

        float closest_t = std::numeric_limits<float>::infinity();
        bool hit = false;

        int count = polygon.points.size();
        for (int i = 0; i < count; ++i)
        {
            Vector a = polygon.points[i];
            Vector b = polygon.points[(i + 1) % count];
            Vector edge = b - a;
            Vector qp = a - ray.origin;
            float denom = ray.direction.x * edge.y - ray.direction.y * edge.x;
            if (std::fabs(denom) < Vector::FLOAT_SMALL)
            {
                continue;
            }
            float t = (qp.x * edge.y - qp.y * edge.x) / denom;
            float u = (qp.x * ray.direction.y - qp.y * ray.direction.x) / denom;
            if (t >= 0.0f && u >= 0.0f && u <= 1.0f && t < closest_t)
            {
                closest_t = t;
                hit = true;
            }
        }

        if (hit)
        {
            collision_point = ray.origin + ray.direction * closest_t;
        }

        return hit;
    }

    bool CollisionChecker::is_point_inside_polygon(const Vector& point, const Polygon& polygon)
    {
        if (polygon.points.size() < 3)
        {
            return false;
        }

        if (point.x < polygon.bbox.min.x || point.x > polygon.bbox.max.x ||
        point.y < polygon.bbox.min.y || point.y > polygon.bbox.max.y)
        {
            return false;
        }

        bool inside = false;
        int count = polygon.points.size();
        for (int i = 0, j = count - 1; i < count; j = i++)
        {
            Vector a = polygon.points[i];
            Vector b = polygon.points[j];

            Vector ab = b - a;
            Vector ap = point - a;
            float cross = ab.x * ap.y - ab.y * ap.x;
            if (std::fabs(cross) < Vector::FLOAT_SMALL &&
            point.x >= std::min(a.x, b.x) && point.x <= std::max(a.x, b.x) &&
            point.y >= std::min(a.y, b.y) && point.y <= std::max(a.y, b.y))
            {
                return true;
            }

            bool cond1 = (a.y > point.y) != (b.y > point.y);
            if (cond1)
            {
                float x_intersect = (b.x - a.x) * (point.y - a.y) / (b.y - a.y) + a.x;
                if (point.x < x_intersect)
                {
                    inside = !inside;
                }
            }
        }

        return inside;
    }

    CollisionResult CollisionChecker::polygon_circle_mtv(const Polygon& polygon, const Circle& circle)
    {
        CollisionResult result;
        auto vertexCount = polygon.points.size();
        if (vertexCount < 3)
        {
            return result;
        }

        if (circle.center.x + circle.radius < polygon.bbox.min.x ||
        circle.center.x - circle.radius > polygon.bbox.max.x ||
        circle.center.y + circle.radius < polygon.bbox.min.y ||
        circle.center.y - circle.radius > polygon.bbox.max.y)
        {
            return result;
        }

        float minOverlap = std::numeric_limits<float>::infinity();
        Vector smallestAxis;

        for (int i = 0; i < vertexCount; ++i)
        {
            Vector edge = polygon.points[(i + 1) % vertexCount] - polygon.points[i];

            Vector axis(-edge.y, edge.x);
            axis.normalize();

            float minPoly = std::numeric_limits<float>::infinity();
            float maxPoly = -std::numeric_limits<float>::infinity();
            for (int j = 0; j < vertexCount; ++j)
            {
                float proj = polygon.points[j].dot(axis);
                if (proj < minPoly) minPoly = proj;
                if (proj > maxPoly) maxPoly = proj;
            }

            float centerProj = circle.center.dot(axis);
            float minCircle = centerProj - circle.radius;
            float maxCircle = centerProj + circle.radius;

            if (maxPoly < minCircle || maxCircle < minPoly)
            return result;

            float overlap = std::min(maxPoly, maxCircle) - std::max(minPoly, minCircle);
            if (overlap < minOverlap)
            {
                minOverlap = overlap;
                smallestAxis = axis;
            }
        }

        {
            float minDistSq = std::numeric_limits<float>::infinity();
            Vector closestVertex;
            for (int i = 0; i < vertexCount; ++i)
            {
                float distSq = polygon.points[i].get_distance_sq(circle.center);
                if (distSq < minDistSq)
                {
                    minDistSq = distSq;
                    closestVertex = polygon.points[i];
                }
            }

            Vector axis = closestVertex - circle.center;
            if (axis.length() > Vector::FLOAT_SMALL)
            {
                axis.normalize();

                float minPoly = std::numeric_limits<float>::infinity();
                float maxPoly = -std::numeric_limits<float>::infinity();
                for (int j = 0; j < vertexCount; ++j)
                {
                    float proj = polygon.points[j].dot(axis);
                    if (proj < minPoly) minPoly = proj;
                    if (proj > maxPoly) maxPoly = proj;
                }

                float centerProj = circle.center.dot(axis);
                float minCircle = centerProj - circle.radius;
                float maxCircle = centerProj + circle.radius;

                if (maxPoly < minCircle || maxCircle < minPoly)
                return result;

                float overlap = std::min(maxPoly, maxCircle) - std::max(minPoly, minCircle);
                if (overlap < minOverlap)
                {
                    minOverlap = overlap;
                    smallestAxis = axis;
                }
            }
        }

        result.collision = true;

        Vector polyCenter(0, 0);
        for (const auto& p : polygon.points) polyCenter += p;
        polyCenter /= (float)polygon.points.size();

        float centerDot = (circle.center - polyCenter).dot(smallestAxis);
        float sign = (centerDot < 0) ? -1.0f : 1.0f;

        result.mtv = smallestAxis * (minOverlap * sign);
        result.penetration = minOverlap;

        return result;
    }

    void CollisionChecker::retain()
    {
        ++this->_reference_counter;
    }

    int CollisionChecker::release()
    {
        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;
    }

    bool CollisionChecker::operator ==(const CollisionChecker& rhs) const
    {
        bool result = true;
        return result;
    }

    bool CollisionChecker::operator !=(const CollisionChecker& rhs) const
    {
        return !(*this == rhs);
    }

    CollisionChecker::CollisionChecker(const CollisionChecker& rhs)
    {
        this->operator=(rhs);
    }

    const CollisionChecker& CollisionChecker::operator =(const CollisionChecker& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string CollisionChecker::get_type() const
    {
        return CollisionChecker::TYPE;
    }

    void CollisionChecker::serialize_xml(SerializerXml& serializer) const
    {

    }

    void CollisionChecker::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void CollisionChecker::serialize_json(SerializerJson& serializer) const
    {

    }

    void CollisionChecker::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
