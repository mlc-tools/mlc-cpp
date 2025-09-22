Используемые проверки:
assertTrue
assertFalse
assertEqual
assertNotEqual
assertNull
assertNotNull
assertInMap
assertNotInMap
assertInList
assertNotInList
assertInRange
assertNotInRange

их объявление:

function void assertTrue(bool expression, string message)
{
    this->add_result(expression, message);
}
function void assertFalse(bool expression, string message)
{
    this->add_result(!expression, message);
}
function<T> void assertEqual(T left, T right, string message)
{
    this->add_result(left == right, message);
}
function<T> void assertNotEqual(T left, T right, string message)
{
    this->add_result(left != right, message);
}
function<T> void assertNull(T value, string message)
{
    this->add_result(value == nullptr, message);
}
function<T> void assertNotNull(T value, string message)
{
    this->add_result(value != nullptr, message);
}
function<Key, Value> void assertInMap(Key key, map<Key, Value>:const:ref map, string message)
{
    this->add_result(in_map(key, map), message);
}
function<Key, Value> void assertNotInMap(Key key, map<Key, Value>:const:ref map, string message)
{
    this->add_result(!in_map(key, map), message);
}
function<T> void assertInList(T item, list<T>:const:ref list, string message)
{
    this->add_result(in_list(item, list), message);
}
function<T> void assertNotInList(T item, list<T>:const:ref list, string message)
{
    this->add_result(!in_list(item, list), message);
}
function<T> void assertInRange(T value, T min_value, T max_value, string message)
{
    bool result = value >= min_value && value <= max_value;
    this->add_result(result, message);
}
function<T> void assertNotInRange(T value, T min_value, T max_value, string message)
{
    bool result = value < min_value || value > max_value;
    this->add_result(result, message);
}

для каждого класса, объявленного в *.mlc файле - необходим свой тестовый класс. Синтаксис тестов:
class Foo
{
    fn int bar()
    {
        return 1;
    }
}
и тест для него:
class TestFoo<ITestFoo>
{
    fn int test_bar()
    {
        Foo foo;
        assertEqual(foo.bar(), 1, "check foo equal 1");
    }
}
ITestFoo - генерируемый интерфейс, его определять не надо. Генерируется для каждого публичного метода из тестируемого класса тестовы метод test_(имя метода). Каждый публичный метод должет иметь свой тест


Для запуска юнит тестов, необходимо:
Поставить зависимости с корневой директории requirements.txt
поставить mlc_tools необходимой версии: python3 tools/mlc_install.py
запустить генерацию, компиляцию и запуск тестов: cd tests/unit_tests/; python3 run.py

для тестирования перегруженных операторов (см. Vector) необходимо создавать отдельные test_operator_add (для operator+), чтобы избежать ошибок компиляции:
tests/unit_tests/mg/tests/ITestVector.h:28:27: error: expected ';' at end of declaration list void test_operator+=();

для сравнения float используй assertTrue(fabs(value - 1.f) < 0.1f), "value equals 1");