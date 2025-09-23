#ifndef __mg_SpawnInfo_h__
#define __mg_SpawnInfo_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "DataUnit.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;


    class SpawnInfo
    {
    public:
        SpawnInfo(const Vector& position=Vector::ZERO, const DataUnit* unit=nullptr);
        ~SpawnInfo();
        void retain();
        int release();
        bool operator ==(const SpawnInfo& rhs) const;
        bool operator !=(const SpawnInfo& rhs) const;
        SpawnInfo(const SpawnInfo& rhs);
        const SpawnInfo& operator =(const SpawnInfo& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        Vector position;
        const DataUnit* unit;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_SpawnInfo_h__
