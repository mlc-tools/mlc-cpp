#ifndef __mg_Modifier_h__
#define __mg_Modifier_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ModifierType.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class Modifier
    {
    public:
        Modifier();
        ~Modifier();
        float modify(float prev_value) const;
        void retain();
        int release();
        bool operator ==(const Modifier& rhs) const;
        bool operator !=(const Modifier& rhs) const;
        Modifier(const Modifier& rhs);
        const Modifier& operator =(const Modifier& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        ModifierType type;
        float value;
        std::string string_value;
        int priority;
        std::string name;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Modifier_h__
