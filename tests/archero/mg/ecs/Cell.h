#ifndef __mg_Cell_h__
#define __mg_Cell_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class Cell
    {
    public:
        Cell(int row=0, int col=0);
        ~Cell();
        void retain();
        int release();
        bool operator ==(const Cell& rhs) const;
        bool operator !=(const Cell& rhs) const;
        Cell(const Cell& rhs);
        const Cell& operator =(const Cell& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        int row;
        int col;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Cell_h__
