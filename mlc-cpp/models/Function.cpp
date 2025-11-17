//
//  Method.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Function.hpp"
#include "Modifier.h"

Function::Function()
    : is_external(false), is_abstract(false), is_template(false),
      is_virtual(false), is_friend(false), is_generate(false),
      translated(false) {}

bool Function::is_equal_declaration(const Function &other) const {
    bool result = Object::is_equal(other);
    return result;
}

void Function::set_modifier(const std::string_view &modifier) {
    if (modifier == Modifier::m_external)
        this->is_external = true;
    else if (modifier == Modifier::m_abstract)
        this->is_abstract = true;
    else if (modifier == Modifier::m_virtual)
        this->is_virtual = true;
    else if (modifier == Modifier::m_generate)
        this->is_generate = true;
    else if (modifier == Modifier::m_binding)
        this->is_binding = true;
    else
        Object::set_modifier(modifier);
}
