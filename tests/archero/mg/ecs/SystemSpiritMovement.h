#ifndef __mg_SystemSpiritMovement_h__
#define __mg_SystemSpiritMovement_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ComponentSpiritBasePoints;
    class ModelEcsBase;
    class Vector;

    class SystemSpiritMovement
    {
    public:
        SystemSpiritMovement();
        ~SystemSpiritMovement();
        void update(ModelEcsBase* model, float dt);
        void initialize_spirit_points(ModelEcsBase* model, ComponentSpiritBasePoints* component_base);
        std::vector<Vector> generate_base_points(ModelEcsBase* model, int count);
        void retain();
        int release();
        bool operator ==(const SystemSpiritMovement& rhs) const;
        bool operator !=(const SystemSpiritMovement& rhs) const;
        SystemSpiritMovement(const SystemSpiritMovement& rhs);
        const SystemSpiritMovement& operator =(const SystemSpiritMovement& rhs);
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
} //namespace mg

#endif //#ifndef __mg_SystemSpiritMovement_h__
