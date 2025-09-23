#ifndef __mg_Shuffle_h__
#define __mg_Shuffle_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../mg_extensions.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class Shuffle
    {
    public:
        Shuffle();
        ~Shuffle();
        static template <class T> T shuffle(const std::map<T, float>& weights, float random_value=-1)
        {

            float total = 0;
            T last;
            bool has_any = false;
            std::vector<float> prefix;
            std::vector<T> keys;

            for (auto&& pair : weights)
            {
                auto& key = pair.first;
                auto& w = pair.second;
                (void)key; //don't generate 'Unused variable' warning
                (void)w; //don't generate 'Unused variable' warning
                float ww = std::max(0.f, w);
                total += ww;
                if(ww > 0)
                {
                    prefix.push_back(total);
                    keys.push_back(key);
                }
                last = key;
                has_any = true;
            }
            if(!has_any)
            {
                return T();
            }
            if(total <= 0)
            {
                return last;
            }

            if(random_value < 0)
            {
                random_value = random_float() * total;
            }

            int l = 0;
            int rgt = prefix.size() - 1;
            while(l < rgt)
            {
                int mid = (l + rgt) / 2;
                if(prefix[mid] >= random_value)
                {
                    rgt = mid;
                }
                else
                {
                    l = mid + 1;
                }
            }
            return keys[l];

        }
        void retain();
        int release();
        bool operator ==(const Shuffle& rhs) const;
        bool operator !=(const Shuffle& rhs) const;
        Shuffle(const Shuffle& rhs);
        const Shuffle& operator =(const Shuffle& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} //namespace mg

#endif //#ifndef __mg_Shuffle_h__
