#ifndef __mg_CompareTestWithPointer_h__
#define __mg_CompareTestWithPointer_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "CompareTestWithoutPointer.h"
#include <atomic>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class CompareTestWithPointer
    {
    public:
        CompareTestWithPointer();
        ~CompareTestWithPointer();
        void retain();
        int release();
        bool operator ==(const CompareTestWithPointer& rhs) const;
        bool operator !=(const CompareTestWithPointer& rhs) const;
        CompareTestWithPointer(const CompareTestWithPointer& rhs);
        const CompareTestWithPointer& operator =(const CompareTestWithPointer& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        intrusive_ptr<CompareTestWithoutPointer> pointer;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_CompareTestWithPointer_h__
