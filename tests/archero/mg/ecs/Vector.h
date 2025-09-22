#ifndef __mg_Vector_h__
#define __mg_Vector_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class Vector
    {
    public:
        Vector(float x=0, float y=0);
        ~Vector();
        Vector operator-() const;
        Vector operator+(const Vector& v) const;
        Vector operator-(const Vector& v) const;
        Vector operator*(float v) const;
        Vector operator/(float v) const;
        Vector& operator+=(const Vector& v);
        Vector& operator-=(const Vector& v);
        Vector& operator*=(float v);
        Vector& operator/=(float v);
        float dot(const Vector& v) const;
        float length() const;
        float length_sq() const;
        Vector& normalize(float desired_length=1);
        Vector get_normalized() const;
        float get_distance(const Vector& v) const;
        float get_distance_sq(const Vector& v) const;
        void rotate(float radians);
        static Vector get_normal_to(const Vector& vector);
        static Vector generate_random_point_on_radius(float radius);
        static Vector generate_random_point_in_radius(float radius);
        static std::vector<Vector> get_points_on_radius(float radius, int count);
        static float get_angle(const Vector& direction);
        static Vector build_vector(float radian);
        static float get_angle_between(const Vector& a, const Vector& b);
        static float get_signed_angle_between(const Vector& a, const Vector& b);
        void retain();
        int release();
        bool operator ==(const Vector& rhs) const;
        bool operator !=(const Vector& rhs) const;
        Vector(const Vector& rhs);
        const Vector& operator =(const Vector& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        static const float FLOAT_SMALL;
        static const Vector ZERO;
        float x;
        float y;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Vector_h__
