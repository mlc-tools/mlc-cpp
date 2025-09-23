#ifndef __mg_BuilderEntity_h__
#define __mg_BuilderEntity_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../model/ModelUser.h"
#include "DataUnit.h"
#include "Side.h"
#include "Vector.h"
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class ModelEcsBase;
    class ModelUser;
    class Side;
    class Vector;

    class BuilderEntity
    {
    public:
        BuilderEntity(const DataUnit* data=nullptr);
        ~BuilderEntity();
        BuilderEntity& set_level(int level);
        BuilderEntity& set_side(Side side);
        BuilderEntity& set_user(ModelUser* user);
        BuilderEntity& set_position(const Vector& pos);
        int build(ModelEcsBase* model);
        void add_components_from_data(ModelEcsBase* model, int id);
        void add_components_from_equipment(ModelEcsBase* model, int id);
        void retain();
        int release();
        bool operator ==(const BuilderEntity& rhs) const;
        bool operator !=(const BuilderEntity& rhs) const;
        BuilderEntity(const BuilderEntity& rhs);
        const BuilderEntity& operator =(const BuilderEntity& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        int id;
        int level;
        Side side;
        Vector position;
        const DataUnit* data;
        intrusive_ptr<ModelUser> user;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_BuilderEntity_h__
