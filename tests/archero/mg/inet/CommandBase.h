#ifndef __mg_CommandBase_h__
#define __mg_CommandBase_h__

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
    class IVisitorResponse;

    class CommandBase
    {
    public:
        CommandBase();
        virtual ~CommandBase();
        virtual void accept(IVisitorResponse* visitor);
        void retain();
        int release();
        bool operator ==(const CommandBase& rhs) const;
        bool operator !=(const CommandBase& rhs) const;
        CommandBase(const CommandBase& rhs);
        const CommandBase& operator =(const CommandBase& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

        int current_time;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_CommandBase_h__
