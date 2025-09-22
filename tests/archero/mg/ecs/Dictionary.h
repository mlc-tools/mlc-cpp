#ifndef __mg_Dictionary_h__
#define __mg_Dictionary_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Vector.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class Vector;

    class Dictionary
    {
    public:
        Dictionary();
        ~Dictionary();
        void retain();
        int release();
        bool operator ==(const Dictionary& rhs) const;
        bool operator !=(const Dictionary& rhs) const;
        Dictionary(const Dictionary& rhs);
        const Dictionary& operator =(const Dictionary& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        std::map<std::string, std::string> strings;
        std::map<std::string, Vector> vectors;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Dictionary_h__
