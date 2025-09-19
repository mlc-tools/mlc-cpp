#ifndef __mg_Logger_h__
#define __mg_Logger_h__

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

    class Logger
    {
    public:
        Logger();
        virtual ~Logger();
        virtual void message(const std::string& message) = 0;
        virtual void log(const std::string& message);
        virtual void retain();
        virtual int release();
        virtual bool operator ==(const Logger& rhs) const;
        virtual bool operator !=(const Logger& rhs) const;
        Logger(const Logger& rhs);
        virtual const Logger& operator =(const Logger& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_Logger_h__
