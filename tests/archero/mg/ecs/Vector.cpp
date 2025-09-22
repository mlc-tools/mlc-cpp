#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Vector.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const float Vector::FLOAT_SMALL(2e-37f);
    const Vector Vector::ZERO;
    const std::string Vector::TYPE("Vector");

    Vector::Vector(float x, float y)
    : x(0)
    , y(0)
    , _reference_counter(1)
    {
        this->x = x;
        this->y = y;
    }

    Vector::~Vector()
    {
    }

    Vector Vector::operator-() const
    {
        return Vector(-this->x, -this->y);
    }

    Vector Vector::operator+(const Vector& v) const
    {
        return Vector(this->x + v.x, this->y + v.y);
    }

    Vector Vector::operator-(const Vector& v) const
    {
        return Vector(this->x - v.x, this->y - v.y);
    }

    Vector Vector::operator*(float v) const
    {
        return Vector(this->x * v, this->y * v);
    }

    Vector Vector::operator/(float v) const
    {
        if(std::fabs(v) > FLOAT_SMALL)
        {
            auto inv = 1.f / v;
            return Vector(this->x * inv, this->y * inv);
        }
        else
        {
            assert(false);
            return Vector(0.f, 0.f);
        }
    }

    Vector& Vector::operator+=(const Vector& v)
    {
        this->x += v.x;
        this->y += v.y;
        return *this;
    }

    Vector& Vector::operator-=(const Vector& v)
    {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
    }

    Vector& Vector::operator*=(float v)
    {
        this->x *= v;
        this->y *= v;
        return *this;
    }

    Vector& Vector::operator/=(float v)
    {
        if(std::fabs(v) > 0.000001f)
        {
            auto inv = 1.f / v;
            this->x *= inv;
            this->y *= inv;
        }
        else
        {
            assert(false);
            this->x = 0;
            this->y = 0;
        }
        return *this;
    }

    float Vector::dot(const Vector& v) const
    {
        return this->x * v.x + this->y * v.y;
    }

    float Vector::length() const
    {
        return std::sqrt(x*x + y*y);
    }

    float Vector::length_sq() const
    {
        return x*x + y*y;
    }

    Vector& Vector::normalize(float desired_length)
    {
        assert(std::fabs(desired_length) > FLOAT_SMALL);
        auto len = this->length();
        if(len > FLOAT_SMALL)
        {
            auto scale = desired_length / len;
            this->x *= scale;
            this->y *= scale;
        }
        else
        {
            this->x = desired_length;
            this->y = 0;
        }
        return *this;
    }

    Vector Vector::get_normalized() const
    {
        Vector v = *this;
        v.normalize();
        return v;
    }

    float Vector::get_distance(const Vector& v) const
    {
        return std::sqrt(this->get_distance_sq(v));
    }

    float Vector::get_distance_sq(const Vector& v) const
    {
        float x = v.x - this->x;
        float y = v.y - this->y;
        return x*x + y*y;
    }

    void Vector::rotate(float radians)
    {
        float x = this->x * cos(radians) - this->y * sin(radians);
        float y = this->x * sin(radians) + this->y * cos(radians);
        this->x = x;
        this->y = y;
    }

    Vector Vector::get_normal_to(const Vector& vector)
    {
        Vector normal(-vector.y, vector.x);
        return normal.normalize();
    }

    Vector Vector::generate_random_point_on_radius(float radius)
    {
        Vector result(random_float() * 2 - 1, random_float() * 2 - 1);
        result.normalize();
        result *= radius;
        return result;
    }

    Vector Vector::generate_random_point_in_radius(float radius)
    {
        Vector result(random_float() * 2 - 1, random_float() * 2 - 1);
        result.normalize();
        result *= random_float() * radius;
        return result;
    }

    std::vector<Vector> Vector::get_points_on_radius(float radius, int count)
    {
        std::vector<Vector> points;
        float pi = std::acos(-1.0f);
        float step = 2.0f * pi / static_cast<float>(count);
        for (int i=0; i<count; ++i)
        {
            float angle = step * static_cast<float>(i);
            float px    = std::cos(angle) * radius;
            float py    = std::sin(angle) * radius;
            points.push_back(Vector(px, py));
        }
        return points;
    }

    float Vector::get_angle(const Vector& direction)
    {
        float rad = std::atan2(direction.y, direction.x);
        if (rad < 0.0f)
        rad += M_PI * 2;
        return rad;
    }

    Vector Vector::build_vector(float radian)
    {
        return Vector(std::cos(radian), std::sin(radian));
    }

    float Vector::get_angle_between(const Vector& a, const Vector& b)
    {
        float dot = a.x * b.x + a.y * b.y;
        float len_prod = a.length() * b.length();
        if (len_prod < FLOAT_SMALL)
        {
            return 0.0f;
        }
        float cosVal = dot / len_prod;
        if (cosVal >  1.0f) cosVal =  1.0f;
        if (cosVal < -1.0f) cosVal = -1.0f;
        float rad = std::acos(cosVal);
        return rad;
    }

    float Vector::get_signed_angle_between(const Vector& a, const Vector& b)
    {
        if (a.length() < FLOAT_SMALL || b.length() < FLOAT_SMALL)
        {
            return 0.0f;
        }
        float cross = a.x * b.y - a.y * b.x;
        float dot   = a.x * b.x + a.y * b.y;
        float rad = std::atan2(cross, dot);
        return rad;
    }

    void Vector::retain()
    {
        ++this->_reference_counter;
    }

    int Vector::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool Vector::operator ==(const Vector& rhs) const
    {
        bool result = true;
        result = result && this->x == rhs.x;
        result = result && this->y == rhs.y;
        return result;
    }

    bool Vector::operator !=(const Vector& rhs) const
    {
        return !(*this == rhs);
    }

    Vector::Vector(const Vector& rhs)
    {
        this->operator=(rhs);
    }

    const Vector& Vector::operator =(const Vector& rhs)
    {
        this->x = rhs.x;
        this->y = rhs.y;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Vector::get_type() const
    {
        return Vector::TYPE;
    }

    void Vector::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(x, "x", float(0));
        serializer.serialize(y, "y", float(0));
    }

    void Vector::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(x, "x", float(0));
        deserializer.deserialize(y, "y", float(0));
    }

    void Vector::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(x, "x", float(0));
        serializer.serialize(y, "y", float(0));
    }

    void Vector::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(x, "x", float(0));
        deserializer.deserialize(y, "y", float(0));
    }

} //namespace mg
