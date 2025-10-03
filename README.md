A code generation utility for C++ and Python based on MLC configs.

### Features:

- Serialization generation 
- Handling constant data (`DataStorage`)
- Interface generation for unit testing
- Reference counters and `intrusive_ptr` generation
- Visitor pattern support (https://en.wikipedia.org/wiki/Visitor_pattern)
- ECS support: https://en.wikipedia.org/wiki/Entity_component_system

### Build:
```
cd ci
./build_and_install.sh
```

### Run tests:
```
cd ci
./tests.sh
```

### Syntax

```
class core/Request<RequestBase>
{
    Action action
    int int_data
    float float_data = 0
    fn void some_function(){}
}
```

- `class`: keyword
- `core/`: module. Used only to group classes into folders for C++ generation
- `Request`: class name
- `RequestBase`: base class. Multiple inheritance is not supported
- `{ ... }`: class or method body
- `int int_data`: integer field named `int_data`
- `float float_data = 0`: float field named `float_data` with an initializer
- `fn void some_function(){}`: class method without a body

