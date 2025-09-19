#include "intrusive_ptr.h"
#include "../mg_Factory.h"
#include "../Observable.h"
#include "../data/DataUnit.h"
#include "ObservableClass.h"
#include <map>
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string ObservableClass::TYPE("ObservableClass");

    
    ObservableClass::ObservableClass()
    : eventTest()
, eventTestInt()
, eventTestInt2()
, eventTestString()
, eventTestString2()
, eventTestDataLink()
, eventTestDataPointer()
, mapEventsVoid()
, mapEventsInt()
, _reference_counter(1)
{
    
    }

    ObservableClass::~ObservableClass(){}


void ObservableClass::retain()
{
++this->_reference_counter;
}

int ObservableClass::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool ObservableClass::operator ==(const ObservableClass& rhs) const
{
bool result = true;
return result;
}

bool ObservableClass::operator !=(const ObservableClass& rhs) const
{

return !(*this == rhs);
}

 ObservableClass::ObservableClass(const ObservableClass& rhs)
{

this->operator=(rhs);
}

const ObservableClass& ObservableClass::operator =(const ObservableClass& rhs)
{

this->eventTest = rhs.eventTest;
this->eventTestInt = rhs.eventTestInt;
this->eventTestInt2 = rhs.eventTestInt2;
this->eventTestString = rhs.eventTestString;
this->eventTestString2 = rhs.eventTestString2;
this->eventTestDataLink = rhs.eventTestDataLink;
this->eventTestDataPointer = rhs.eventTestDataPointer;
this->mapEventsVoid = rhs.mapEventsVoid;
this->mapEventsInt = rhs.mapEventsInt;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string ObservableClass::get_type() const
{
return ObservableClass::TYPE;
}

void ObservableClass::serialize_xml(SerializerXml& serializer) const
{

}

void ObservableClass::deserialize_xml(DeserializerXml& deserializer)
{

}

void ObservableClass::serialize_json(SerializerJson& serializer) const
{

}

void ObservableClass::deserialize_json(DeserializerJson& deserializer)
{

}

} // namespace mg
