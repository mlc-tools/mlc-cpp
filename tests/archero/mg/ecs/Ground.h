#ifndef __mg_Ground_h__
#define __mg_Ground_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Cell.h"
#include "Polygon.h"
#include "Vector.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class Cell;
    class Polygon;
    class Vector;

    class Ground
    {
    public:
        Ground();
        ~Ground();
        int get_width_tiles() const;
        int get_height_tiles() const;
        Vector get_center_position() const;
        Cell convert_position_to_tile(const Vector& pos) const;
        Vector conver_tile_to_position(const Cell& cell) const;
        bool is_bound(const Vector& pos) const;
        bool has_collision_with_walls(const Vector& pos, float radius) const;
        void retain();
        int release();
        bool operator ==(const Ground& rhs) const;
        bool operator !=(const Ground& rhs) const;
        Ground(const Ground& rhs);
        const Ground& operator =(const Ground& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        static const int WALL;
        static const int GROUND;
        std::vector<std::vector<int>> ground;
        std::vector<Polygon> wall_polygons;
        float tile_width;
        float tile_heigth;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Ground_h__
