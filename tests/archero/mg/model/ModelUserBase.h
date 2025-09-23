#ifndef __mg_ModelUserBase_h__
#define __mg_ModelUserBase_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "Model.h"
#include "System.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
    class System;

    class ModelUserBase : public Model
    {
    public:
        ModelUserBase();
        virtual ~ModelUserBase();
        template <class T> intrusive_ptr<T> system()
        {

            for (auto&& pair : this->systems)
            {
                auto& type = pair.first;
                auto& system = pair.second;
                (void)type; //don't generate 'Unused variable' warning
                (void)system; //don't generate 'Unused variable' warning
                if(type == T::TYPE)
                {
                    auto result = dynamic_pointer_cast_intrusive<T>(system);

                    return result;
                }
            }
            return nullptr;

        }
        template <class T> const T* system() const
        {

            for (auto&& pair : this->systems)
            {
                auto& type = pair.first;
                auto& system = pair.second;
                (void)type; //don't generate 'Unused variable' warning
                (void)system; //don't generate 'Unused variable' warning
                if(type == T::TYPE)
                {
                    auto result = dynamic_pointer_cast_intrusive<T>(system);
                    assert(result != nullptr);
                    return result;
                }
            }
            return nullptr;

        }
        void copy_system(System* system);
        std::string get_auth_key() const;
        bool operator ==(const ModelUserBase& rhs) const;
        bool operator !=(const ModelUserBase& rhs) const;
        ModelUserBase(const ModelUserBase& rhs);
        const ModelUserBase& operator =(const ModelUserBase& rhs);
        virtual std::string get_type() const override;
        virtual void serialize_xml(SerializerXml& serializer) const override;
        virtual void deserialize_xml(DeserializerXml& deserializer) override;
        virtual void serialize_json(SerializerJson& serializer) const override;
        virtual void deserialize_json(DeserializerJson& deserializer) override;

        int id;
    private:
        std::string auth_key;
    public:
        std::string auth_key_for_tests;
    protected:
        std::map<std::string, intrusive_ptr<System>> systems;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_ModelUserBase_h__
