#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Cell.h"
#include "CollisionChecker.h"
#include "Ground.h"
#include "Polygon.h"
#include "Vector.h"
#include <vector>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const int Ground::WALL(1);
    const int Ground::GROUND(0);
    const std::string Ground::TYPE("Ground");

    Ground::Ground()
    : ground()
    , wall_polygons()
    , tile_width(64)
    , tile_heigth(64)
    , _reference_counter(1)
    {

    }

    Ground::~Ground()
    {
    }

    int Ground::get_width_tiles() const
    {
        return ground.at(0).size();
    }

    int Ground::get_height_tiles() const
    {
        return ground.size();
    }

    Vector Ground::get_center_position() const
    {
        auto width = tile_width * ground[0].size();
        auto height = tile_heigth * ground.size();
        return Vector(width / 2, height / 2);
    }

    Cell Ground::convert_position_to_tile(const Vector& pos) const
    {
        Cell cell;
        cell.row = pos.y / this->tile_heigth;
        cell.col = pos.x / this->tile_width;
        return cell;
    }

    Vector Ground::conver_tile_to_position(const Cell& cell) const
    {
        return Vector(cell.col * this->tile_width, cell.row * this->tile_heigth);
    }

    bool Ground::is_bound(const Vector& pos) const
    {
        auto width = tile_width * ground[0].size();
        auto height = tile_heigth * ground.size();
        return pos.x > 0 && pos.x < width && pos.y > 0 && pos.y < height;
    }

    bool Ground::has_collision_with_walls(const Vector& pos, float radius) const
    {
        auto tile = this->convert_position_to_tile(pos);
        if(this->ground.size() == 0)
        {
            return false;
        }
        if(tile.row < 0 || tile.row >= list_size(this->ground) || tile.col < 0 || tile.col >= list_size(this->ground[0]))
        {
            return true;
        }
        if(this->ground[tile.row][tile.col] == WALL)
        {
            return true;
        }
        for(auto& polygon : this->wall_polygons)
        {
            if(CollisionChecker::is_point_inside_polygon(pos, polygon))
            {
                return true;
            }
        }
        return false;
    }

    void Ground::retain()
    {
        ++this->_reference_counter;
    }

    int Ground::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool Ground::operator ==(const Ground& rhs) const
    {
        bool result = true;
        result = result && this->ground == rhs.ground;
        result = result && this->wall_polygons == rhs.wall_polygons;
        result = result && this->tile_width == rhs.tile_width;
        result = result && this->tile_heigth == rhs.tile_heigth;
        return result;
    }

    bool Ground::operator !=(const Ground& rhs) const
    {
        return !(*this == rhs);
    }

    Ground::Ground(const Ground& rhs)
    {
        this->operator=(rhs);
    }

    const Ground& Ground::operator =(const Ground& rhs)
    {
        this->ground = rhs.ground;
        this->wall_polygons = rhs.wall_polygons;
        this->tile_width = rhs.tile_width;
        this->tile_heigth = rhs.tile_heigth;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Ground::get_type() const
    {
        return Ground::TYPE;
    }

    void Ground::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(ground, "ground");
        serializer.serialize(wall_polygons, "wall_polygons");
        serializer.serialize(tile_width, "tile_width", float(64));
        serializer.serialize(tile_heigth, "tile_heigth", float(64));
    }

    void Ground::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(ground, "ground");
        deserializer.deserialize(wall_polygons, "wall_polygons");
        deserializer.deserialize(tile_width, "tile_width", float(64));
        deserializer.deserialize(tile_heigth, "tile_heigth", float(64));
    }

    void Ground::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(ground, "ground");
        serializer.serialize(wall_polygons, "wall_polygons");
        serializer.serialize(tile_width, "tile_width", float(64));
        serializer.serialize(tile_heigth, "tile_heigth", float(64));
    }

    void Ground::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(ground, "ground");
        deserializer.deserialize(wall_polygons, "wall_polygons");
        deserializer.deserialize(tile_width, "tile_width", float(64));
        deserializer.deserialize(tile_heigth, "tile_heigth", float(64));
    }

} //namespace mg
