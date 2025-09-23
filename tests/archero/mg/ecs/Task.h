#ifndef __mg_Task_h__
#define __mg_Task_h__

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


    class Task
    {
    public:
        Task();
        ~Task();
        void retain();
        int release();
        bool operator ==(const Task& rhs) const;
        bool operator !=(const Task& rhs) const;
        Task(const Task& rhs);
        const Task& operator =(const Task& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Task_h__
