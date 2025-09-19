#ifndef __mg_RunAllTests_h__
#define __mg_RunAllTests_h__

#include <cstdint>
#include "intrusive_ptr.h"
#include "pugixml/pugixml.hpp"
#include "../TestClassMissingTestMethod.h"
#include "../TestConstructableClass.h"
#include "../TestDataAdd.h"
#include "../TestDataComplexMap.h"
#include "../TestDataListListBool.h"
#include "../TestFooEnumTestParent.h"
#include "../TestFunctionTemplates.h"
#include "../TestIntrusive.h"
#include "../TestNullishOperator.h"
#include "../TestRequestAcceptor.h"
#include "../TestSerializeTest.h"
#include "../TestStaticPointer.h"
#include "../TestTemplateMethod.h"
#include "../TestTranslates.h"
#include "../clone/TestCloneObject.h"
#include "../compare/TestCompareTestWithoutPointer.h"
#include "../data/TestDataDict.h"
#include "../inner/TestOuterClass.h"
#include "../inner/TestOuterClassInnerClass.h"
#include "../observable/TestObservableClass.h"
#include <string>


namespace mg
{
    class SerializerXml;
    class DeserializerXml;
    class SerializerJson;
    class DeserializerJson;
class Logger;

    class RunAllTests
    {
    public:
        RunAllTests();
        ~RunAllTests();
void initialize(Logger* logger);
bool execute();
void retain();
int release();
bool operator ==(const RunAllTests& rhs) const;
bool operator !=(const RunAllTests& rhs) const;
 RunAllTests(const RunAllTests& rhs);
const RunAllTests& operator =(const RunAllTests& rhs);
std::string get_type() const;
void serialize_xml(SerializerXml& serializer) const;
void deserialize_xml(DeserializerXml& deserializer);
void serialize_json(SerializerJson& serializer) const;
void deserialize_json(DeserializerJson& deserializer);

TestRequestAcceptor test_request_acceptor;
TestDataComplexMap test_data_complex_map;
TestDataListListBool test_data_list_list_bool;
TestStaticPointer test_static_pointer;
TestClassMissingTestMethod test_class_missing_test_method;
TestObservableClass test_observable_class;
TestConstructableClass test_constructable_class;
TestTranslates test_translates;
TestNullishOperator test_nullish_operator;
TestIntrusive test_intrusive;
TestDataDict test_data_dict;
TestSerializeTest test_serialize_test;
TestCompareTestWithoutPointer test_compare_test_without_pointer;
TestFooEnumTestParent test_foo_enum_test_parent;
TestCloneObject test_clone_object;
TestOuterClass test_outer_class;
TestOuterClassInnerClass test_outer_class_inner_class;
TestFunctionTemplates test_function_templates;
TestTemplateMethod test_template_method;
TestDataAdd test_data_add;
private:
int _reference_counter;
public:
static const std::string TYPE;

    };
} // namespace mg

#endif // __mg_RunAllTests_h__
