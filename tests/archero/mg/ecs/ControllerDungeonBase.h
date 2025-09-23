#ifndef __mg_ControllerDungeonBase_h__
#define __mg_ControllerDungeonBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "IControllerDungeonBase.h"
#include "ModelEcsBase.h"
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class DataStatUpgrade;
    class DataUnit;
    class Vector;

    class ControllerDungeonBase : public IControllerDungeonBase
    {
    public:
        ControllerDungeonBase();
        virtual ~ControllerDungeonBase();
        virtual void pause();
        virtual void resume();
        virtual bool is_level_finished();
        virtual bool is_wave_finished();
        virtual void update(float dt);
        virtual void create_hero();
        virtual void create_gate_to_next_wave();
        virtual void generate_wave();
        virtual void generate_units_on_wave();
        virtual void create_enemy(const DataUnit* unit, const Vector& pos, int level);
        virtual void add_skill(const DataStatUpgrade* skill);
        virtual void add_skill_to_entity(const DataStatUpgrade* skill, int entity_id);
        virtual std::vector<const DataStatUpgrade*> generate_skills(int count);
        virtual bool operator ==(const ControllerDungeonBase& rhs) const;
        virtual bool operator !=(const ControllerDungeonBase& rhs) const;
        ControllerDungeonBase(const ControllerDungeonBase& rhs);
        virtual const ControllerDungeonBase& operator =(const ControllerDungeonBase& rhs);
        virtual std::string get_type() const;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        intrusive_ptr<ModelEcsBase> model;
        int paused;
        int wave_counter;
        bool show_window_skill;
        bool run_next_wave;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ControllerDungeonBase_h__
