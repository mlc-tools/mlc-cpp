#ifndef __mg_DataStorage_h__
#define __mg_DataStorage_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class DataEquipment;
    class DataLadderLevels;
    class DataLevel;
    class DataLocale;
    class DataMergeLevel;
    class DataParams;
    class DataStatUpgrade;
    class DataUnit;

    class DataStorage
    {
    public:
        DataStorage();
        ~DataStorage();
        std::vector<std::string> get_levels_keys() const;
        std::vector<std::string> get_units_keys() const;
        std::vector<std::string> get_stat_upgrades_keys() const;
        std::vector<std::string> get_ladder_levelses_keys() const;
        std::vector<std::string> get_locales_keys() const;
        std::vector<std::string> get_paramses_keys() const;
        std::vector<std::string> get_equipments_keys() const;
        std::vector<std::string> get_merge_levels_keys() const;
        static const DataStorage& shared();
        void initialize_xml(const std::string& content) const;
        template <class T> const T* get(const std::string& name) const;
        const std::map<std::string, DataLevel>& get_levels() const;
        const std::map<std::string, DataUnit>& get_units() const;
        const std::map<std::string, DataStatUpgrade>& get_stat_upgrades() const;
        const std::map<std::string, DataLadderLevels>& get_ladder_levelses() const;
        const std::map<std::string, DataLocale>& get_locales() const;
        const std::map<std::string, DataParams>& get_paramses() const;
        const std::map<std::string, DataEquipment>& get_equipments() const;
        const std::map<std::string, DataMergeLevel>& get_merge_levels() const;
        void retain();
        int release();
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

    private:
        std::map<std::string, DataLevel> levels;
        std::map<std::string, DataUnit> units;
        std::map<std::string, DataStatUpgrade> stat_upgrades;
        std::map<std::string, DataLadderLevels> ladder_levelses;
        std::map<std::string, DataLocale> locales;
        std::map<std::string, DataParams> paramses;
        std::map<std::string, DataEquipment> equipments;
        std::map<std::string, DataMergeLevel> merge_levels;
        bool _loaded;
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataStorage_h__
