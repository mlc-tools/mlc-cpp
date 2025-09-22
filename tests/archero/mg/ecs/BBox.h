#ifndef __mg_BBox_h__
#define __mg_BBox_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
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

    class BBox
    {
    public:
        BBox();
        ~BBox();
        void update(const std::vector<Vector>& points);
        void retain();
        int release();
        bool operator ==(const BBox& rhs) const;
        bool operator !=(const BBox& rhs) const;
        BBox(const BBox& rhs);
        const BBox& operator =(const BBox& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        Vector min;
        Vector max;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_BBox_h__
