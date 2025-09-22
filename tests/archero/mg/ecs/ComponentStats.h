#ifndef __mg_ComponentStats_h__
#define __mg_ComponentStats_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "ComponentBase.h"
#include "DataStatUpgrade.h"
#include "DataUnit.h"
#include "Modifier.h"
#include "UnitStat.h"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class DataStatUpgrade;
    class ModelEcsBase;
    class Modifier;
    class UnitStat;

    class ComponentStats : public ComponentBase
    {
    public:
        ComponentStats();
        virtual ~ComponentStats();
        void add_upgrade(const DataStatUpgrade* data);
        void set_fixed(const UnitStat& stat, float value);
        void reset_cashe(const UnitStat& stat);
        void remove_upgrade(const DataStatUpgrade* data);
        float get_stat_base(const UnitStat& stat);
    private:
        std::string get_stat_string_base(const UnitStat& stat);
    public:
        float get(ModelEcsBase* model, const UnitStat& stat);
        std::string get_string(ModelEcsBase* model, const UnitStat& stat);
        bool operator ==(const ComponentStats& rhs) const;
        bool operator !=(const ComponentStats& rhs) const;
        ComponentStats(const ComponentStats& rhs);
        const ComponentStats& operator =(const ComponentStats& rhs);
        virtual std::string get_type() const override;
        virtual void add_self_to_model(ModelEcsBase* model_dungeon_class) override;
        virtual void remove_self_from_model(ModelEcsBase* model_dungeon_class) override;
        virtual intrusive_ptr<ComponentBase> get_self_from_model(ModelEcsBase* model_dungeon_class, int id);
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        int parent_id;
        const DataUnit* data;
        std::vector<const DataStatUpgrade*> upgrades;
    private:
        std::map<UnitStat, std::vector<Modifier>> stats;
        std::map<UnitStat, float> cashe;
        std::map<UnitStat, std::string> cashe_string;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ComponentStats_h__
