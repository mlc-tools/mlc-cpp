#ifndef __mg_DataUnitVisual_h__
#define __mg_DataUnitVisual_h__

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


    class DataUnitVisual
    {
    public:
        DataUnitVisual();
        ~DataUnitVisual();
        void retain();
        int release();
        bool operator ==(const DataUnitVisual& rhs) const;
        bool operator !=(const DataUnitVisual& rhs) const;
        DataUnitVisual(const DataUnitVisual& rhs);
        const DataUnitVisual& operator =(const DataUnitVisual& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string path_to_view;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataUnitVisual_h__
