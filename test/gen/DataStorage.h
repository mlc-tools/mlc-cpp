#ifndef __mg_DataStorage_h__
#define __mg_DataStorage_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "DataAdd.h"
#include "DataComplexMap.h"
#include "DataListListBool.h"
#include "data/DataDict.h"
#include "data/DataNonExist.h"
#include "data/DataUnit.h"
#include "data/LadderLevels.h"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class DataStorage
    {
    public:
        DataStorage();
        ~DataStorage();
        std::vector<std::string> get_complex_maps_keys() const;
        std::vector<std::string> get_list_list_bools_keys() const;
        std::vector<std::string> get_non_exists_keys() const;
        std::vector<std::string> get_ladder_levelss_keys() const;
        std::vector<std::string> get_units_keys() const;
        std::vector<std::string> get_dicts_keys() const;
        std::vector<std::string> get_adds_keys() const;
        static const DataStorage& shared();
        void initialize_xml(const std::string& content) const;
        template <class T> const T* get(const std::string& name) const;
        const std::map<std::string, DataComplexMap>& get_complex_maps() const;
        const std::map<std::string, DataListListBool>& get_list_list_bools() const;
        const std::map<std::string, DataNonExist>& get_non_exists() const;
        const std::map<std::string, LadderLevels>& get_ladder_levelss() const;
        const std::map<std::string, DataUnit>& get_units() const;
        const std::map<std::string, DataDict>& get_dicts() const;
        const std::map<std::string, DataAdd>& get_adds() const;
        void retain();
        int release();
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

    private:
        std::map<std::string, DataComplexMap> complex_maps;
        std::map<std::string, DataListListBool> list_list_bools;
        std::map<std::string, DataNonExist> non_exists;
        std::map<std::string, LadderLevels> ladder_levelss;
        std::map<std::string, DataUnit> units;
        std::map<std::string, DataDict> dicts;
        std::map<std::string, DataAdd> adds;
        bool _loaded;
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_DataStorage_h__
