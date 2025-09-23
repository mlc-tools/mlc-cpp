#ifndef __mg_Model_h__
#define __mg_Model_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class Model
    {
    public:
        Model();
        virtual ~Model();
        virtual void initialize(int current_time);
        bool is_outdate(int current_time);
        void retain();
        int release();
        bool operator ==(const Model& rhs) const;
        bool operator !=(const Model& rhs) const;
        Model(const Model& rhs);
        const Model& operator =(const Model& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const;
        virtual void deserialize_xml(DeserializerXml& deserializer);
        virtual void serialize_json(SerializerJson& serializer) const;
        virtual void deserialize_json(DeserializerJson& deserializer);

        bool initialized;
        int timestamp;
        bool ready_to_write;

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Model_h__
