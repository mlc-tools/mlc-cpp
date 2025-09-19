#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "CompareTestBase.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string CompareTestBase::TYPE("CompareTestBase");

    
    CompareTestBase::CompareTestBase()
    : id(0)
, _reference_counter(1)
{
    
    }

    CompareTestBase::~CompareTestBase(){}


void CompareTestBase::retain()
{
++this->_reference_counter;
}

int CompareTestBase::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool CompareTestBase::operator ==(const CompareTestBase& rhs) const
{
bool result = true;
result = result && this->id == rhs.id;
return result;
}

bool CompareTestBase::operator !=(const CompareTestBase& rhs) const
{

return !(*this == rhs);
}

 CompareTestBase::CompareTestBase(const CompareTestBase& rhs)
{

this->operator=(rhs);
}

const CompareTestBase& CompareTestBase::operator =(const CompareTestBase& rhs)
{

this->id = rhs.id;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string CompareTestBase::get_type() const
{
return CompareTestBase::TYPE;
}

void CompareTestBase::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(id, "id", int(0));
}

void CompareTestBase::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(id, "id", int(0));
}

void CompareTestBase::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(id, "id", int(0));
}

void CompareTestBase::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(id, "id", int(0));
}

} // namespace mg
