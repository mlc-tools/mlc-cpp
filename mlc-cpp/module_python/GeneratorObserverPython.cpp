//
//  GeneratorObserverPython.cpp
//

#include "GeneratorObserverPython.hpp"
#include "../models/Class.hpp"
#include "../models/Model.hpp"

static const char *PREDEFINED = R"PY(
class @{name}:

    def __init__(self):
        self._listeners = {}
        self._add = {}
        self._remove = []
        self._locked = 0

    def _is_locked(self):
        return self._locked != 0

    def _lock(self):
        self._locked += 1

    def _unlock(self):
        self._locked -= 1
        if not self._is_locked():
            for object in self._add:
                self._listeners[object] = self._add[object]
            for object in self._remove:
                del self._listeners[object]
            self._add = {}
            self._remove = []

    def _call(self, obj, func, *args):
        def isclass(obj_):
            return isinstance(obj_, object)

        if isclass(obj):
            func(obj, *args)
        else:
            func(*args)

    def add(self, object, functor):
        if self._is_locked():
            if object in self._remove:
                self._remove.remove(object)
            else:
                self._add[object] = functor
        else:
            self._listeners[object] = functor

    def remove(self, object):
        if self._is_locked():
            self._remove.append(object)
        else:
            del self._listeners[object]

    def notify(self, *args):
        self._lock()
        for object in self._listeners:
            if object not in self._remove:
                self._call(object, self._listeners[object], *args)
        self._unlock()
)PY";

std::string GeneratorObserverPython::getObservableName() {
    return std::string("Observable");
}

void GeneratorObserverPython::generate(Model &model) {
    std::string text = PREDEFINED;
    const std::string name = getObservableName();
    // replace markers
    size_t pos;
    while ((pos = text.find("@{name}")) != std::string::npos)
        text.replace(pos, 7, name);
    // write file
    model.addFile(nullptr, name + ".py", text);

    // add mock non-generated class into model to satisfy references
    auto cls = std::make_shared<Class>();
    cls->name = name;
    cls->type = "class";
    cls->auto_generated = false;
    model.add_class(cls);
}
