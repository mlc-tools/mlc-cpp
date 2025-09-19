#ifndef __mg_ObservableClass_h__
#define __mg_ObservableClass_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../Observable.h"
#include "../data/DataUnit.h"
#include <map>
#include <string>

namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;

    class ObservableClass
    {
    public:
        ObservableClass();
        ~ObservableClass();
        void retain();
        int release();
        bool operator ==(const ObservableClass& rhs) const;
        bool operator !=(const ObservableClass& rhs) const;
        ObservableClass(const ObservableClass& rhs);
        const ObservableClass& operator =(const ObservableClass& rhs);
        std::string get_type() const;
        void serialize_xml(SerializerXml& serializer) const;
        void deserialize_xml(DeserializerXml& deserializer);
        void serialize_json(SerializerJson& serializer) const;
        void deserialize_json(DeserializerJson& deserializer);

        Observable<void()> eventTest;
        Observable<void(int)> eventTestInt;
        Observable<void(int, int)> eventTestInt2;
        Observable<void(int, const std::string&)> eventTestString;
        Observable<void(const std::string&)> eventTestString2;
        Observable<void(const DataUnit*)> eventTestDataLink;
        Observable<void(DataUnit*)> eventTestDataPointer;
        std::map<int, Observable<void()>> mapEventsVoid;
        std::map<int, Observable<void(int)>> mapEventsInt;
    private:
        int _reference_counter;
    public:
        static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_ObservableClass_h__
