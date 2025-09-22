#ifndef __mg_VisualItem_h__
#define __mg_VisualItem_h__

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

    class VisualItem
    {
    public:
        VisualItem();
        ~VisualItem();
        void retain();
        int release();
        bool operator ==(const VisualItem& rhs) const;
        bool operator !=(const VisualItem& rhs) const;
        VisualItem(const VisualItem& rhs);
        const VisualItem& operator =(const VisualItem& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::string name;
        std::string desc;
        std::string icon;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_VisualItem_h__
