#ifndef __mg_ModelCity_h__
#define __mg_ModelCity_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ModelBase.h"
#include <map>
#include <string>
#include <vector>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class ModelCity : public ModelBase
    {
    public:
        ModelCity();
        ~ModelCity();
int some_function(const std::string& resource);
private:
int some_private_function(const std::string& resource);
public:
bool operator ==(const ModelCity& rhs) const;
bool operator !=(const ModelCity& rhs) const;
 ModelCity(const ModelCity& rhs);
const ModelCity& operator =(const ModelCity& rhs);
virtual std::string get_type() const;
virtual void serialize_xml(SerializerXml& serializer) const override;
virtual void deserialize_xml(DeserializerXml& deserializer) override;
virtual void serialize_json(SerializerJson& serializer) const override;
virtual void deserialize_json(DeserializerJson& deserializer) override;

private:
int id;
public:
int owner;
static std::map<std::string, std::string> test_map;
static std::vector<std::string> test_list;
static ModelBase model_base_static;
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ModelCity_h__
