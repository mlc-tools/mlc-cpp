#ifndef __mg_CommandSequence_h__
#define __mg_CommandSequence_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "CommandBase.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class CommandBase;
    class IVisitorResponse;

    class CommandSequence : public CommandBase
    {
    public:
        CommandSequence();
        virtual ~CommandSequence();
        virtual void accept(IVisitorResponse* visitor) override;
        bool operator ==(const CommandSequence& rhs) const;
        bool operator !=(const CommandSequence& rhs) const;
        CommandSequence(const CommandSequence& rhs);
        const CommandSequence& operator =(const CommandSequence& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        std::vector<intrusive_ptr<CommandBase>> commands;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_CommandSequence_h__
