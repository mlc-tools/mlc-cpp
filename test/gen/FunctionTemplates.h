#ifndef __mg_FunctionTemplates_h__
#define __mg_FunctionTemplates_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "FunctionTemplates.h"
#include "FunctionTemplatesSomeOtherClass.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class FunctionTemplates
    {
    public:
        FunctionTemplates();
        ~FunctionTemplates();
        template <class T> T add(T a, T b)
        {

            return a + b;

        }
        template <class T> void use_other_class(T t)
        {

            FunctionTemplatesSomeOtherClass obj;
            obj.some_method();

        }
        void retain();
        int release();
        bool operator ==(const FunctionTemplates& rhs) const;
        bool operator !=(const FunctionTemplates& rhs) const;
        FunctionTemplates(const FunctionTemplates& rhs);
        const FunctionTemplates& operator =(const FunctionTemplates& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_FunctionTemplates_h__
