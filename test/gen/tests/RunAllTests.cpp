#include "intrusive_ptr.h"
#include "../mg_Factory.h"
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
#include "Logger.h"
#include "RunAllTests.h"
#include <string>
#include "../mg_extensions.h"
#include "../SerializerJson.h"
#include "../SerializerXml.h"

namespace mg
{
    const std::string RunAllTests::TYPE("RunAllTests");

    
    RunAllTests::RunAllTests()
    : test_request_acceptor()
, test_data_complex_map()
, test_data_list_list_bool()
, test_static_pointer()
, test_class_missing_test_method()
, test_observable_class()
, test_constructable_class()
, test_translates()
, test_nullish_operator()
, test_intrusive()
, test_data_dict()
, test_serialize_test()
, test_compare_test_without_pointer()
, test_foo_enum_test_parent()
, test_clone_object()
, test_outer_class()
, test_outer_class_inner_class()
, test_function_templates()
, test_template_method()
, test_data_add()
, _reference_counter(1)
{
    
    }

    RunAllTests::~RunAllTests(){}


void RunAllTests::initialize(Logger* logger)
{
this->test_request_acceptor.initialize(logger);this->test_data_complex_map.initialize(logger);this->test_data_list_list_bool.initialize(logger);this->test_static_pointer.initialize(logger);this->test_class_missing_test_method.initialize(logger);this->test_observable_class.initialize(logger);this->test_constructable_class.initialize(logger);this->test_translates.initialize(logger);this->test_nullish_operator.initialize(logger);this->test_intrusive.initialize(logger);this->test_data_dict.initialize(logger);this->test_serialize_test.initialize(logger);this->test_compare_test_without_pointer.initialize(logger);this->test_foo_enum_test_parent.initialize(logger);this->test_clone_object.initialize(logger);this->test_outer_class.initialize(logger);this->test_outer_class_inner_class.initialize(logger);this->test_function_templates.initialize(logger);this->test_template_method.initialize(logger);this->test_data_add.initialize(logger);
}

bool RunAllTests::execute()
{
this->test_request_acceptor.execute();this->test_data_complex_map.execute();this->test_data_list_list_bool.execute();this->test_static_pointer.execute();this->test_class_missing_test_method.execute();this->test_observable_class.execute();this->test_constructable_class.execute();this->test_translates.execute();this->test_nullish_operator.execute();this->test_intrusive.execute();this->test_data_dict.execute();this->test_serialize_test.execute();this->test_compare_test_without_pointer.execute();this->test_foo_enum_test_parent.execute();this->test_clone_object.execute();this->test_outer_class.execute();this->test_outer_class_inner_class.execute();this->test_function_templates.execute();this->test_template_method.execute();this->test_data_add.execute();bool result = true;result = result && this->test_request_acceptor.result;result = result && this->test_data_complex_map.result;result = result && this->test_data_list_list_bool.result;result = result && this->test_static_pointer.result;result = result && this->test_class_missing_test_method.result;result = result && this->test_observable_class.result;result = result && this->test_constructable_class.result;result = result && this->test_translates.result;result = result && this->test_nullish_operator.result;result = result && this->test_intrusive.result;result = result && this->test_data_dict.result;result = result && this->test_serialize_test.result;result = result && this->test_compare_test_without_pointer.result;result = result && this->test_foo_enum_test_parent.result;result = result && this->test_clone_object.result;result = result && this->test_outer_class.result;result = result && this->test_outer_class_inner_class.result;result = result && this->test_function_templates.result;result = result && this->test_template_method.result;result = result && this->test_data_add.result;return result;
}

void RunAllTests::retain()
{
++this->_reference_counter;
}

int RunAllTests::release()
{

--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;

}

bool RunAllTests::operator ==(const RunAllTests& rhs) const
{
bool result = true;
result = result && this->test_request_acceptor == rhs.test_request_acceptor;
result = result && this->test_data_complex_map == rhs.test_data_complex_map;
result = result && this->test_data_list_list_bool == rhs.test_data_list_list_bool;
result = result && this->test_static_pointer == rhs.test_static_pointer;
result = result && this->test_class_missing_test_method == rhs.test_class_missing_test_method;
result = result && this->test_observable_class == rhs.test_observable_class;
result = result && this->test_constructable_class == rhs.test_constructable_class;
result = result && this->test_translates == rhs.test_translates;
result = result && this->test_nullish_operator == rhs.test_nullish_operator;
result = result && this->test_intrusive == rhs.test_intrusive;
result = result && this->test_data_dict == rhs.test_data_dict;
result = result && this->test_serialize_test == rhs.test_serialize_test;
result = result && this->test_compare_test_without_pointer == rhs.test_compare_test_without_pointer;
result = result && this->test_foo_enum_test_parent == rhs.test_foo_enum_test_parent;
result = result && this->test_clone_object == rhs.test_clone_object;
result = result && this->test_outer_class == rhs.test_outer_class;
result = result && this->test_outer_class_inner_class == rhs.test_outer_class_inner_class;
result = result && this->test_function_templates == rhs.test_function_templates;
result = result && this->test_template_method == rhs.test_template_method;
result = result && this->test_data_add == rhs.test_data_add;
return result;
}

bool RunAllTests::operator !=(const RunAllTests& rhs) const
{

return !(*this == rhs);
}

 RunAllTests::RunAllTests(const RunAllTests& rhs)
{

this->operator=(rhs);
}

const RunAllTests& RunAllTests::operator =(const RunAllTests& rhs)
{

this->test_request_acceptor = rhs.test_request_acceptor;
this->test_data_complex_map = rhs.test_data_complex_map;
this->test_data_list_list_bool = rhs.test_data_list_list_bool;
this->test_static_pointer = rhs.test_static_pointer;
this->test_class_missing_test_method = rhs.test_class_missing_test_method;
this->test_observable_class = rhs.test_observable_class;
this->test_constructable_class = rhs.test_constructable_class;
this->test_translates = rhs.test_translates;
this->test_nullish_operator = rhs.test_nullish_operator;
this->test_intrusive = rhs.test_intrusive;
this->test_data_dict = rhs.test_data_dict;
this->test_serialize_test = rhs.test_serialize_test;
this->test_compare_test_without_pointer = rhs.test_compare_test_without_pointer;
this->test_foo_enum_test_parent = rhs.test_foo_enum_test_parent;
this->test_clone_object = rhs.test_clone_object;
this->test_outer_class = rhs.test_outer_class;
this->test_outer_class_inner_class = rhs.test_outer_class_inner_class;
this->test_function_templates = rhs.test_function_templates;
this->test_template_method = rhs.test_template_method;
this->test_data_add = rhs.test_data_add;
this->_reference_counter = rhs._reference_counter;
return *this;
}

std::string RunAllTests::get_type() const
{
return RunAllTests::TYPE;
}

void RunAllTests::serialize_xml(SerializerXml& serializer) const
{
serializer.serialize(test_request_acceptor, "test_request_acceptor");
serializer.serialize(test_data_complex_map, "test_data_complex_map");
serializer.serialize(test_data_list_list_bool, "test_data_list_list_bool");
serializer.serialize(test_static_pointer, "test_static_pointer");
serializer.serialize(test_class_missing_test_method, "test_class_missing_test_method");
serializer.serialize(test_observable_class, "test_observable_class");
serializer.serialize(test_constructable_class, "test_constructable_class");
serializer.serialize(test_translates, "test_translates");
serializer.serialize(test_nullish_operator, "test_nullish_operator");
serializer.serialize(test_intrusive, "test_intrusive");
serializer.serialize(test_data_dict, "test_data_dict");
serializer.serialize(test_serialize_test, "test_serialize_test");
serializer.serialize(test_compare_test_without_pointer, "test_compare_test_without_pointer");
serializer.serialize(test_foo_enum_test_parent, "test_foo_enum_test_parent");
serializer.serialize(test_clone_object, "test_clone_object");
serializer.serialize(test_outer_class, "test_outer_class");
serializer.serialize(test_outer_class_inner_class, "test_outer_class_inner_class");
serializer.serialize(test_function_templates, "test_function_templates");
serializer.serialize(test_template_method, "test_template_method");
serializer.serialize(test_data_add, "test_data_add");
}

void RunAllTests::deserialize_xml(DeserializerXml& deserializer)
{
deserializer.deserialize(test_request_acceptor, "test_request_acceptor");
deserializer.deserialize(test_data_complex_map, "test_data_complex_map");
deserializer.deserialize(test_data_list_list_bool, "test_data_list_list_bool");
deserializer.deserialize(test_static_pointer, "test_static_pointer");
deserializer.deserialize(test_class_missing_test_method, "test_class_missing_test_method");
deserializer.deserialize(test_observable_class, "test_observable_class");
deserializer.deserialize(test_constructable_class, "test_constructable_class");
deserializer.deserialize(test_translates, "test_translates");
deserializer.deserialize(test_nullish_operator, "test_nullish_operator");
deserializer.deserialize(test_intrusive, "test_intrusive");
deserializer.deserialize(test_data_dict, "test_data_dict");
deserializer.deserialize(test_serialize_test, "test_serialize_test");
deserializer.deserialize(test_compare_test_without_pointer, "test_compare_test_without_pointer");
deserializer.deserialize(test_foo_enum_test_parent, "test_foo_enum_test_parent");
deserializer.deserialize(test_clone_object, "test_clone_object");
deserializer.deserialize(test_outer_class, "test_outer_class");
deserializer.deserialize(test_outer_class_inner_class, "test_outer_class_inner_class");
deserializer.deserialize(test_function_templates, "test_function_templates");
deserializer.deserialize(test_template_method, "test_template_method");
deserializer.deserialize(test_data_add, "test_data_add");
}

void RunAllTests::serialize_json(SerializerJson& serializer) const
{
serializer.serialize(test_request_acceptor, "test_request_acceptor");
serializer.serialize(test_data_complex_map, "test_data_complex_map");
serializer.serialize(test_data_list_list_bool, "test_data_list_list_bool");
serializer.serialize(test_static_pointer, "test_static_pointer");
serializer.serialize(test_class_missing_test_method, "test_class_missing_test_method");
serializer.serialize(test_observable_class, "test_observable_class");
serializer.serialize(test_constructable_class, "test_constructable_class");
serializer.serialize(test_translates, "test_translates");
serializer.serialize(test_nullish_operator, "test_nullish_operator");
serializer.serialize(test_intrusive, "test_intrusive");
serializer.serialize(test_data_dict, "test_data_dict");
serializer.serialize(test_serialize_test, "test_serialize_test");
serializer.serialize(test_compare_test_without_pointer, "test_compare_test_without_pointer");
serializer.serialize(test_foo_enum_test_parent, "test_foo_enum_test_parent");
serializer.serialize(test_clone_object, "test_clone_object");
serializer.serialize(test_outer_class, "test_outer_class");
serializer.serialize(test_outer_class_inner_class, "test_outer_class_inner_class");
serializer.serialize(test_function_templates, "test_function_templates");
serializer.serialize(test_template_method, "test_template_method");
serializer.serialize(test_data_add, "test_data_add");
}

void RunAllTests::deserialize_json(DeserializerJson& deserializer)
{
deserializer.deserialize(test_request_acceptor, "test_request_acceptor");
deserializer.deserialize(test_data_complex_map, "test_data_complex_map");
deserializer.deserialize(test_data_list_list_bool, "test_data_list_list_bool");
deserializer.deserialize(test_static_pointer, "test_static_pointer");
deserializer.deserialize(test_class_missing_test_method, "test_class_missing_test_method");
deserializer.deserialize(test_observable_class, "test_observable_class");
deserializer.deserialize(test_constructable_class, "test_constructable_class");
deserializer.deserialize(test_translates, "test_translates");
deserializer.deserialize(test_nullish_operator, "test_nullish_operator");
deserializer.deserialize(test_intrusive, "test_intrusive");
deserializer.deserialize(test_data_dict, "test_data_dict");
deserializer.deserialize(test_serialize_test, "test_serialize_test");
deserializer.deserialize(test_compare_test_without_pointer, "test_compare_test_without_pointer");
deserializer.deserialize(test_foo_enum_test_parent, "test_foo_enum_test_parent");
deserializer.deserialize(test_clone_object, "test_clone_object");
deserializer.deserialize(test_outer_class, "test_outer_class");
deserializer.deserialize(test_outer_class_inner_class, "test_outer_class_inner_class");
deserializer.deserialize(test_function_templates, "test_function_templates");
deserializer.deserialize(test_template_method, "test_template_method");
deserializer.deserialize(test_data_add, "test_data_add");
}

} // namespace mg
