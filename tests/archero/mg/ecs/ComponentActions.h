#ifndef __mg_ComponentActions_h__
#define __mg_ComponentActions_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ActionBase.h"
#include "ComponentBase.h"
#include "Dictionary.h"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ActionBase;
    class ModelEcsBase;

    class ComponentActions : public ComponentBase
    {
    public:
        ComponentActions();
        virtual ~ComponentActions();
        intrusive_ptr<ActionBase> get_action(const std::string& name);
        void initialize(ModelEcsBase* model, int entity_id);
        bool operator ==(const ComponentActions& rhs) const;
        bool operator !=(const ComponentActions& rhs) const;
        ComponentActions(const ComponentActions& rhs);
        const ComponentActions& operator =(const ComponentActions& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        bool initialized;
    private:
        std::vector<intrusive_ptr<ActionBase>> actions;
        std::map<std::string, intrusive_ptr<ActionBase>> map_actions;
    public:
        intrusive_ptr<ActionBase> current_action;
        std::string first_action;
        std::string next_action;
        intrusive_ptr<Dictionary> dictionary;
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentActions_h__
