#ifndef __mg_StaticPointer_h__
#define __mg_StaticPointer_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "core/CommandBase.h"
#include <atomic>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class StaticPointer
    {
    public:
        StaticPointer();
        ~StaticPointer();
        void retain();
        int release();
        bool operator ==(const StaticPointer& rhs) const;
        bool operator !=(const StaticPointer& rhs) const;
        StaticPointer(const StaticPointer& rhs);
        const StaticPointer& operator =(const StaticPointer& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        static intrusive_ptr<CommandBase> command;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_StaticPointer_h__
