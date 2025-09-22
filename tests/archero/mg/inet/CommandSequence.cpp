#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "CommandBase.h"
#include "CommandSequence.h"
#include "IVisitorResponse.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string CommandSequence::TYPE("CommandSequence");

    CommandSequence::CommandSequence()
    : commands()
    {

    }

    CommandSequence::~CommandSequence()
    {
    }

    void CommandSequence::accept(IVisitorResponse* visitor)
    {
        for(auto command : this->commands)
        command->accept(visitor);
    }

    bool CommandSequence::operator ==(const CommandSequence& rhs) const
    {
        bool result = this->CommandBase::operator ==(rhs);
        result = result && this->commands == rhs.commands;
        return result;
    }

    bool CommandSequence::operator !=(const CommandSequence& rhs) const
    {
        return !(*this == rhs);
    }

    CommandSequence::CommandSequence(const CommandSequence& rhs)
    {
        this->operator=(rhs);
    }

    const CommandSequence& CommandSequence::operator =(const CommandSequence& rhs)
    {
        this->CommandBase::operator=(rhs);
        this->commands = rhs.commands;
        return *this;
    }

    std::string CommandSequence::get_type() const
    {
        return CommandSequence::TYPE;
    }

    void CommandSequence::serialize_xml(SerializerXml& serializer) const
    {
        CommandBase::serialize_xml(serializer);
        serializer.serialize(commands, "commands");
    }

    void CommandSequence::deserialize_xml(DeserializerXml& deserializer)
    {
        CommandBase::deserialize_xml(deserializer);
        deserializer.deserialize(commands, "commands");
    }

    void CommandSequence::serialize_json(SerializerJson& serializer) const
    {
        CommandBase::serialize_json(serializer);
        serializer.serialize(commands, "commands");
    }

    void CommandSequence::deserialize_json(DeserializerJson& deserializer)
    {
        CommandBase::deserialize_json(deserializer);
        deserializer.deserialize(commands, "commands");
    }

} //namespace mg
