
#include "Registrar.h"
#include "mg_Factory.h"
#include "AllTests.h"
#include "core/CommandBase.h"
#include "AllTypesChildren.h"
#include "AllTypes.h"
#include "Request.h"
#include "RequestFoo.h"
#include "RequestBar.h"
#include "RequestBar2.h"
#include "RequestAcceptor.h"
#include "TestRequestAcceptor.h"
#include "Response.h"
#include "TestEnumValue1.h"
#include "TestEnumValue2.h"
#include "TestToFor.h"
#include "FunctionTest.h"
#include "TestUser1.h"
#include "TestFactory.h"
#include "DataComplexMap.h"
#include "DataListListBool.h"
#include "TestDataComplexMap.h"
#include "TestDataListListBool.h"
#include "StaticPointer.h"
#include "TestStaticPointer.h"
#include "ClassMissingTestMethod.h"
#include "TestClassMissingTestMethod.h"
#include "observable/ObservableClass.h"
#include "observable/ListenerClass.h"
#include "observable/TestObservableClass.h"
#include "ConstructableClass.h"
#include "ConstructableClassWithParameters.h"
#include "ConstructableClassAuto.h"
#include "TestConstructableClass.h"
#include "Translates.h"
#include "Point.h"
#include "TestTranslates.h"
#include "NullishOperator.h"
#include "TestNullishOperator.h"
#include "TestTranslatesModel.h"
#include "model/ModelCity.h"
#include "Intrusive.h"
#include "TestIntrusive.h"
#include "data/DataNonExist.h"
#include "SkillBase.h"
#include "SkillFoo.h"
#include "data/LadderLevels.h"
#include "data/DataUnit.h"
#include "data/VisualUnit.h"
#include "TestDataAdd.h"
#include "data/DataDict.h"
#include "data/TestDataDict.h"
#include "ExceptionTest.h"
#include "SerializeTest.h"
#include "TestSerializeTest.h"
#include "compare/CompareTestBase.h"
#include "compare/CompareTestWithoutPointer.h"
#include "compare/CompareTestWithPointer.h"
#include "compare/TestCompareTestWithoutPointer.h"
#include "test/TestIncludesGet.h"
#include "test/TestIncludes.h"
#include "SideTestBase.h"
#include "SideTestServer.h"
#include "SideTestClient.h"
#include "SideTestCommon.h"
#include "FooEnumTestParent.h"
#include "TestFooEnumTestParent.h"
#include "clone/CloneObject.h"
#include "clone/CloneObject2.h"
#include "clone/TestCloneObject.h"
#include "inner/OuterClass.h"
#include "inner/TestOuterClass.h"
#include "inner/TestOuterClassInnerClass.h"
#include "OuterClassInnerClass.h"
#include "FunctionTemplates.h"
#include "FunctionTemplatesSomeOtherClass.h"
#include "TestFunctionTemplates.h"
#include "TemplateMethod.h"
#include "BaseClass.h"
#include "FooClassFromBase.h"
#include "BarClassFromBase.h"
#include "TestTemplateMethod.h"
#include "DataAdd.h"
#include "tests/ITestRequestAcceptor.h"
#include "tests/ITestStaticPointer.h"
#include "tests/ITestObservableClass.h"
#include "tests/ITestConstructableClass.h"
#include "tests/ITestDataDict.h"
#include "tests/ITestSerializeTest.h"
#include "tests/RunAllTests.h"
#include "DataStorage.h"

void mg::register_classes()
{
    Factory::shared().registrationCommand<AllTests>(AllTests::TYPE);
    Factory::shared().registrationCommand<CommandBase>(CommandBase::TYPE);
    Factory::shared().registrationCommand<AllTypesChildren>(AllTypesChildren::TYPE);
    Factory::shared().registrationCommand<AllTypes>(AllTypes::TYPE);
    Factory::shared().registrationCommand<Request>(Request::TYPE);
    Factory::shared().registrationCommand<RequestFoo>(RequestFoo::TYPE);
    Factory::shared().registrationCommand<RequestBar>(RequestBar::TYPE);
    Factory::shared().registrationCommand<RequestBar2>(RequestBar2::TYPE);
    Factory::shared().registrationCommand<RequestAcceptor>(RequestAcceptor::TYPE);
    Factory::shared().registrationCommand<TestRequestAcceptor>(TestRequestAcceptor::TYPE);
    Factory::shared().registrationCommand<Response>(Response::TYPE);
    Factory::shared().registrationCommand<TestEnumValue1>(TestEnumValue1::TYPE);
    Factory::shared().registrationCommand<TestEnumValue2>(TestEnumValue2::TYPE);
    Factory::shared().registrationCommand<TestToFor>(TestToFor::TYPE);
    Factory::shared().registrationCommand<FunctionTest>(FunctionTest::TYPE);
    Factory::shared().registrationCommand<TestUser1>(TestUser1::TYPE);
    Factory::shared().registrationCommand<TestFactory>(TestFactory::TYPE);
    Factory::shared().registrationCommand<DataComplexMap>(DataComplexMap::TYPE);
    Factory::shared().registrationCommand<DataListListBool>(DataListListBool::TYPE);
    Factory::shared().registrationCommand<TestDataComplexMap>(TestDataComplexMap::TYPE);
    Factory::shared().registrationCommand<TestDataListListBool>(TestDataListListBool::TYPE);
    Factory::shared().registrationCommand<StaticPointer>(StaticPointer::TYPE);
    Factory::shared().registrationCommand<TestStaticPointer>(TestStaticPointer::TYPE);
    Factory::shared().registrationCommand<ClassMissingTestMethod>(ClassMissingTestMethod::TYPE);
    Factory::shared().registrationCommand<TestClassMissingTestMethod>(TestClassMissingTestMethod::TYPE);
    Factory::shared().registrationCommand<ObservableClass>(ObservableClass::TYPE);
    Factory::shared().registrationCommand<ListenerClass>(ListenerClass::TYPE);
    Factory::shared().registrationCommand<TestObservableClass>(TestObservableClass::TYPE);
    Factory::shared().registrationCommand<ConstructableClass>(ConstructableClass::TYPE);
    Factory::shared().registrationCommand<ConstructableClassWithParameters>(ConstructableClassWithParameters::TYPE);
    Factory::shared().registrationCommand<ConstructableClassAuto>(ConstructableClassAuto::TYPE);
    Factory::shared().registrationCommand<TestConstructableClass>(TestConstructableClass::TYPE);
    Factory::shared().registrationCommand<Translates>(Translates::TYPE);
    Factory::shared().registrationCommand<Point>(Point::TYPE);
    Factory::shared().registrationCommand<TestTranslates>(TestTranslates::TYPE);
    Factory::shared().registrationCommand<NullishOperator>(NullishOperator::TYPE);
    Factory::shared().registrationCommand<TestNullishOperator>(TestNullishOperator::TYPE);
    Factory::shared().registrationCommand<TestTranslatesModel>(TestTranslatesModel::TYPE);
    Factory::shared().registrationCommand<ModelCity>(ModelCity::TYPE);
    Factory::shared().registrationCommand<Intrusive>(Intrusive::TYPE);
    Factory::shared().registrationCommand<TestIntrusive>(TestIntrusive::TYPE);
    Factory::shared().registrationCommand<DataNonExist>(DataNonExist::TYPE);
    Factory::shared().registrationCommand<SkillBase>(SkillBase::TYPE);
    Factory::shared().registrationCommand<SkillFoo>(SkillFoo::TYPE);
    Factory::shared().registrationCommand<LadderLevels>(LadderLevels::TYPE);
    Factory::shared().registrationCommand<DataUnit>(DataUnit::TYPE);
    Factory::shared().registrationCommand<VisualUnit>(VisualUnit::TYPE);
    Factory::shared().registrationCommand<TestDataAdd>(TestDataAdd::TYPE);
    Factory::shared().registrationCommand<DataDict>(DataDict::TYPE);
    Factory::shared().registrationCommand<TestDataDict>(TestDataDict::TYPE);
    Factory::shared().registrationCommand<ExceptionTest>(ExceptionTest::TYPE);
    Factory::shared().registrationCommand<SerializeTest>(SerializeTest::TYPE);
    Factory::shared().registrationCommand<TestSerializeTest>(TestSerializeTest::TYPE);
    Factory::shared().registrationCommand<CompareTestBase>(CompareTestBase::TYPE);
    Factory::shared().registrationCommand<CompareTestWithoutPointer>(CompareTestWithoutPointer::TYPE);
    Factory::shared().registrationCommand<CompareTestWithPointer>(CompareTestWithPointer::TYPE);
    Factory::shared().registrationCommand<TestCompareTestWithoutPointer>(TestCompareTestWithoutPointer::TYPE);
    Factory::shared().registrationCommand<TestIncludesGet>(TestIncludesGet::TYPE);
    Factory::shared().registrationCommand<TestIncludes>(TestIncludes::TYPE);
    Factory::shared().registrationCommand<SideTestBase>(SideTestBase::TYPE);
    Factory::shared().registrationCommand<SideTestServer>(SideTestServer::TYPE);
    Factory::shared().registrationCommand<SideTestClient>(SideTestClient::TYPE);
    Factory::shared().registrationCommand<SideTestCommon>(SideTestCommon::TYPE);
    Factory::shared().registrationCommand<FooEnumTestParent>(FooEnumTestParent::TYPE);
    Factory::shared().registrationCommand<TestFooEnumTestParent>(TestFooEnumTestParent::TYPE);
    Factory::shared().registrationCommand<CloneObject>(CloneObject::TYPE);
    Factory::shared().registrationCommand<CloneObject2>(CloneObject2::TYPE);
    Factory::shared().registrationCommand<TestCloneObject>(TestCloneObject::TYPE);
    Factory::shared().registrationCommand<OuterClass>(OuterClass::TYPE);
    Factory::shared().registrationCommand<TestOuterClass>(TestOuterClass::TYPE);
    Factory::shared().registrationCommand<TestOuterClassInnerClass>(TestOuterClassInnerClass::TYPE);
    Factory::shared().registrationCommand<OuterClassInnerClass>(OuterClassInnerClass::TYPE);
    Factory::shared().registrationCommand<FunctionTemplates>(FunctionTemplates::TYPE);
    Factory::shared().registrationCommand<FunctionTemplatesSomeOtherClass>(FunctionTemplatesSomeOtherClass::TYPE);
    Factory::shared().registrationCommand<TestFunctionTemplates>(TestFunctionTemplates::TYPE);
    Factory::shared().registrationCommand<TemplateMethod>(TemplateMethod::TYPE);
    Factory::shared().registrationCommand<BaseClass>(BaseClass::TYPE);
    Factory::shared().registrationCommand<FooClassFromBase>(FooClassFromBase::TYPE);
    Factory::shared().registrationCommand<BarClassFromBase>(BarClassFromBase::TYPE);
    Factory::shared().registrationCommand<TestTemplateMethod>(TestTemplateMethod::TYPE);
    Factory::shared().registrationCommand<DataAdd>(DataAdd::TYPE);
    Factory::shared().registrationCommand<ITestRequestAcceptor>(ITestRequestAcceptor::TYPE);
    Factory::shared().registrationCommand<ITestStaticPointer>(ITestStaticPointer::TYPE);
    Factory::shared().registrationCommand<ITestObservableClass>(ITestObservableClass::TYPE);
    Factory::shared().registrationCommand<ITestConstructableClass>(ITestConstructableClass::TYPE);
    Factory::shared().registrationCommand<ITestDataDict>(ITestDataDict::TYPE);
    Factory::shared().registrationCommand<ITestSerializeTest>(ITestSerializeTest::TYPE);
    Factory::shared().registrationCommand<RunAllTests>(RunAllTests::TYPE);
    Factory::shared().registrationCommand<DataStorage>(DataStorage::TYPE);

}
