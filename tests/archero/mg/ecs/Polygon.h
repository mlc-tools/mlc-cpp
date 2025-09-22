#ifndef __mg_Polygon_h__
#define __mg_Polygon_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "BBox.h"
#include "Vector.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class Vector;

    class Polygon
    {
    public:
        Polygon();
        ~Polygon();
        bool is_convex_polygon();
        void retain();
        int release();
        bool operator ==(const Polygon& rhs) const;
        bool operator !=(const Polygon& rhs) const;
        Polygon(const Polygon& rhs);
        const Polygon& operator =(const Polygon& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::vector<Vector> points;
        BBox bbox;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Polygon_h__
