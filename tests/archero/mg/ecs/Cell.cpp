#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Cell.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Cell::TYPE("Cell");

    Cell::Cell(int row, int col)
    : row(0)
    , col(0)
    , _reference_counter(1)
    {
        this->row = row;
        this->col = col;
    }

    Cell::~Cell()
    {
    }

    void Cell::retain()
    {
        ++this->_reference_counter;
    }

    int Cell::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool Cell::operator ==(const Cell& rhs) const
    {
        bool result = true;
        result = result && this->row == rhs.row;
        result = result && this->col == rhs.col;
        return result;
    }

    bool Cell::operator !=(const Cell& rhs) const
    {
        return !(*this == rhs);
    }

    Cell::Cell(const Cell& rhs)
    {
        this->operator=(rhs);
    }

    const Cell& Cell::operator =(const Cell& rhs)
    {
        this->row = rhs.row;
        this->col = rhs.col;
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Cell::get_type() const
    {
        return Cell::TYPE;
    }

    void Cell::serialize_xml(SerializerXml& serializer) const
    {
        serializer.serialize(row, "row", int(0));
        serializer.serialize(col, "col", int(0));
    }

    void Cell::deserialize_xml(DeserializerXml& deserializer)
    {
        deserializer.deserialize(row, "row", int(0));
        deserializer.deserialize(col, "col", int(0));
    }

    void Cell::serialize_json(SerializerJson& serializer) const
    {
        serializer.serialize(row, "row", int(0));
        serializer.serialize(col, "col", int(0));
    }

    void Cell::deserialize_json(DeserializerJson& deserializer)
    {
        deserializer.deserialize(row, "row", int(0));
        deserializer.deserialize(col, "col", int(0));
    }

} //namespace mg
