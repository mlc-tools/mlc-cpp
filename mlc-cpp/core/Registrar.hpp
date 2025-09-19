//
//  Registrar.hpp
//  mlc-cpp
//
//  C++ port of the Python Registrar that generates
//  Registrar.h/.cpp with explicit factory registrations
//  when auto-registration is disabled.
//

#pragma once

#include <memory>
#include <string>

class Model;
struct Class;

class Registrar {
public:
    Registrar() = default;
    void generate(Model &model);

    // Helper: create mock class descriptor (top-level, no group)
    static std::shared_ptr<Class> get_mock();

private:
    static std::string get_include_path_to_class(const std::shared_ptr<Class>& from,
                                                 const std::shared_ptr<Class>& to);
};

