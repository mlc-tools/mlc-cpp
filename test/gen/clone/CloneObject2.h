#ifndef __mg_CloneObject2_h__
#define __mg_CloneObject2_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../data/DataUnit.h"
#include "CloneObject.h"
#include <atomic>
#include <map>
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class CloneObject2 : public CloneObject
    {
    public:
        CloneObject2();
        ~CloneObject2();
void foo();
bool operator ==(const CloneObject2& rhs) const;
bool operator !=(const CloneObject2& rhs) const;
 CloneObject2(const CloneObject2& rhs);
const CloneObject2& operator =(const CloneObject2& rhs);
virtual std::string get_type() const override;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

intrusive_ptr<CloneObject> ptr;
std::vector<intrusive_ptr<CloneObject>> list1;
std::vector<const DataUnit*> list2;
std::map<intrusive_ptr<CloneObject>, intrusive_ptr<CloneObject>> map1;
std::map<intrusive_ptr<CloneObject>, const DataUnit*> map2;
std::map<const DataUnit*, intrusive_ptr<CloneObject>> map3;
std::map<const DataUnit*, const DataUnit*> map4;
std::map<intrusive_ptr<CloneObject>, int> map5;
std::map<int, intrusive_ptr<CloneObject>> map6;
std::map<const DataUnit*, int> map7;
std::map<int, const DataUnit*> map8;
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_CloneObject2_h__
