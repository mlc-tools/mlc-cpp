#ifndef __mg_FooClassFromBase_h__
#define __mg_FooClassFromBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "BaseClass.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class FooClassFromBase : public BaseClass
    {
    public:
        FooClassFromBase();
        ~FooClassFromBase();
        bool operator ==(const FooClassFromBase& rhs) const;
        bool operator !=(const FooClassFromBase& rhs) const;
        FooClassFromBase(const FooClassFromBase& rhs);
        const FooClassFromBase& operator =(const FooClassFromBase& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_FooClassFromBase_h__
