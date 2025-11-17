#include "Bindings.h"
#include "DataTestBindings.h"
#include "data/DataUnit.h"
#include "DataStorage.h"
#include "Observable.h"
#include "models/ModelUser.h"
#include "models/SystemFoo.h"
#include "mg_extensions.h"
#include <unordered_map>

using namespace mg;

bool tests_bindigs()
{
    auto user = make_intrusive<ModelUser>();
    user->initialize();
    
    bool result = true;
    assert(strTo<int>(get_binding_value("DataTestBindings::get_int", "name=test_bindings")) == 1);
    assert(get_binding_value("DataTestBindings::get_name", "name=test_bindings") == "test_bindings");
    assert(get_binding_value("SystemFoo::get_foo", "") == "123");

    std::cout << "Bindings tests: " << (result ? "Success" : "Failed") << std::endl;
    
    int v = 0;
    mg::Observable<void()> *observable = get_binding_observable("ModelUser::event_foo_changed");
    assert(observable);
    observable->add(1, [&](){
        v = 1;
    });
    ModelUser::get_instance()->event_foo_changed.notify();
    assert(v == 1);
    
    user->uninitialize();
    return result;
}
