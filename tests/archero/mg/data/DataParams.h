#ifndef __mg_DataParams_h__
#define __mg_DataParams_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../ecs/SkillRarity.h"
#include "../model/System.h"
#include <map>
#include <string>
#include <vector>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class SkillRarity;
    class System;

    class DataParams
    {
    public:
        DataParams();
        ~DataParams();
        void retain();
        int release();
        bool operator ==(const DataParams& rhs) const;
        bool operator !=(const DataParams& rhs) const;
        DataParams(const DataParams& rhs);
        const DataParams& operator =(const DataParams& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        static const std::string DEFAULT;
        static std::string SERVER_VERSION_ON_CLIENT;
        static std::string SERVER_VERSION_ON_SERVER;
        std::string name;
        std::string app_package;
        bool crypt_resources;
        std::map<std::string, bool> features;
        std::vector<intrusive_ptr<System>> user_systems;
        std::map<SkillRarity, float> skill_rarity_weights;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_DataParams_h__
