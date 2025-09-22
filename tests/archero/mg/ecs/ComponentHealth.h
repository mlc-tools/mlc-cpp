#ifndef __mg_ComponentHealth_h__
#define __mg_ComponentHealth_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;

    class ComponentHealth : public ComponentBase
    {
    public:
        ComponentHealth();
        virtual ~ComponentHealth();
        float get_current() const;
        void set_current(float value);
        float get_future_damage(int damager);
        void add_future_damage(int damager, float damage);
        void remove_future_damage(int damager);
        void damage(int damager, float damage);
    private:
        void calculate_future_damage();
    public:
        float get_future_value() const;
        bool operator ==(const ComponentHealth& rhs) const;
        bool operator !=(const ComponentHealth& rhs) const;
        ComponentHealth(const ComponentHealth& rhs);
        const ComponentHealth& operator =(const ComponentHealth& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

    private:
        float current;
    public:
        float max;
    private:
        std::map<int, float> future_damage;
        float sum_future_damage;
    public:
        int ressurection_count;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentHealth_h__
