#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "CompareTestWithPointer.h"
#include "CompareTestWithoutPointer.h"
#include <atomic>
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string CompareTestWithPointer::TYPE("CompareTestWithPointer");

    
    CompareTestWithPointer::CompareTestWithPointer()
    : pointer(nullptr)
, _reference_counter(1)
{
    
    }

    CompareTestWithPointer::~CompareTestWithPointer(){}


void CompareTestWithPointer::retain()
{
++this->_reference_counter;
}

int CompareTestWithPointer::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool CompareTestWithPointer::operator ==(const CompareTestWithPointer& rhs) const
{
bool result = true;
result = result && ((this->pointer == rhs.pointer) || (this->pointer != nullptr && rhs.pointer != nullptr && *this->pointer == *rhs.pointer));
return result;
}

bool CompareTestWithPointer::operator !=(const CompareTestWithPointer& rhs) const
{

return !(*this == rhs);
}

 CompareTestWithPointer::CompareTestWithPointer(const CompareTestWithPointer& rhs)
{

this->operator=(rhs);
}

const CompareTestWithPointer& CompareTestWithPointer::operator =(const CompareTestWithPointer& rhs)
{

this->pointer = rhs.pointer;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string CompareTestWithPointer::get_type() const
{
return CompareTestWithPointer::TYPE;
}

void CompareTestWithPointer::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(pointer, "pointer");
}

void CompareTestWithPointer::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(pointer, "pointer");
}

void CompareTestWithPointer::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(pointer, "pointer");
}

void CompareTestWithPointer::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(pointer, "pointer");
}

} // namespace mg
