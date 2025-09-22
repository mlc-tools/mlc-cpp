#ifndef __mg_SystemMovement_h__
#define __mg_SystemMovement_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../Observable.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ComponentMovement;
    class ComponentStats;
    class ModelEcsBase;
    class Transform;
    class Vector;

    class SystemMovement
    {
    public:
        SystemMovement();
        ~SystemMovement();
        void update(ModelEcsBase* model, float dt);
        void clean(ModelEcsBase* model);
        void move_by_direction(ModelEcsBase* model, float dt);
        void move_to_target(ModelEcsBase* model, float dt);
        void move_by_parabolic(ModelEcsBase* model, float dt);
        void move_instant(ModelEcsBase* model, float dt);
        void move_vertical(ModelEcsBase* model, float dt);
        bool move(ModelEcsBase* model, ComponentMovement* movement, Transform* transform, ComponentStats* stats, const Vector& offset);
        void retain();
        int release();
        bool operator ==(const SystemMovement& rhs) const;
        bool operator !=(const SystemMovement& rhs) const;
        SystemMovement(const SystemMovement& rhs);
        const SystemMovement& operator =(const SystemMovement& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        static std::map<int, Observable<void()>> event_on_wall;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_SystemMovement_h__
