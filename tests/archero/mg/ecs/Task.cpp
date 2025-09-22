#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "Task.h"
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string Task::TYPE("Task");

    Task::Task()
    : _reference_counter(1)
    {

    }

    Task::~Task()
    {
    }

    void Task::retain()
    {
        ++this->_reference_counter;
    }

    int Task::release()
    {

        --this->_reference_counter;
        auto counter = this->_reference_counter;
        if(counter == 0)
        {
            delete this;
        }
        return counter;

    }

    bool Task::operator ==(const Task& rhs) const
    {
        bool result = true;
        return result;
    }

    bool Task::operator !=(const Task& rhs) const
    {
        return !(*this == rhs);
    }

    Task::Task(const Task& rhs)
    {
        this->operator=(rhs);
    }

    const Task& Task::operator =(const Task& rhs)
    {
        this->_reference_counter = rhs._reference_counter;
        return *this;
    }

    std::string Task::get_type() const
    {
        return Task::TYPE;
    }

    void Task::serialize_xml(SerializerXml& serializer) const
    {

    }

    void Task::deserialize_xml(DeserializerXml& deserializer)
    {

    }

    void Task::serialize_json(SerializerJson& serializer) const
    {

    }

    void Task::deserialize_json(DeserializerJson& deserializer)
    {

    }

} //namespace mg
