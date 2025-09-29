//
//  Error.cpp
//  mlc-cpp
//

#include "Error.hpp"
#include <iostream>
#include <cstdlib>
#include <unordered_map>

bool Log::use_colors = true;
bool Log::disable_logs = false;

static inline std::string colorize(const std::string& msg, const char* color) {
    return std::string(color) + msg + Color::end;
}

void Log::debug(const std::string& msg) {
    if (!disable_logs)
        std::cout << (use_colors ? colorize(msg, Color::light_grey) : msg) << std::endl;
}
void Log::error(const std::string& msg) {
    std::cerr << (use_colors ? colorize(msg, Color::red) : msg) << std::endl;
}
void Log::warning(const std::string& msg) {
    if (!disable_logs)
        std::cout << (use_colors ? colorize(msg, Color::orange) : msg) << std::endl;
}
void Log::message(const std::string& msg) {
    if (!disable_logs)
        std::cout << (use_colors ? colorize(msg, Color::green) : msg) << std::endl;
}

static const std::unordered_map<Error::Code, std::string> kTexts = {
    { Error::UNKNOWN_SERIALISED_TYPE, "[{}] unknown serialized serialized format. Base type - [{}]" },
    { Error::STATIS_MEMBER_SHOULD_HAVE_INITIALISATION, "Static method have to have a initial value. [{}::{}]" },
    { Error::MAP_TWO_ARGS, "Map have to have 2 arguments. [{}::{}]" },
    { Error::DUBLICATE_METHODS, "duplication function in one class [{}::{}]" },
    { Error::DUBLICATE_CLASS, "Duplication classes [{}]" },
    { Error::UNKNOWN_SUPERCLASS, "Cannot find superclass class: {}<{}>" },
    { Error::UNKNOWN_CLASS, "Unknown class: {}" },
    { Error::CANNOT_FIND_CLASS_FOR_METHOD, "Cannot find class [{}] for method [{}]" },
    { Error::CANNOT_FIND_CLASS_FOR_OBJECT, "Cannot find class [{}] for object [{}]" },
    { Error::OBJECT_IS_KEY_OF_MAP, "Validate php feature: key of array cannot be object [{}::map<{}, {}> {}]" },
    { Error::CANNOT_PARSE_XML, "Error on parsing xml [{}]" },
    { Error::ENUM_CANNOT_BE_COMBINATED, "Enum member [{}::{}] cannot be initialed by [{}] value" },
    { Error::INTERNAL_ERROR, "Internal error" },
    { Error::WARNING_SYNTAX, "Syntax warning: Found symbol \";\" in member declaration: [{}]" },
    { Error::UNKNOWN_DATA_TYPE, "Unknown data type [{}]->[{}]. please check configuration. File: [{}]" },
    { Error::CLASS_HAVE_MORE_THAN_ONE_CONSTRUCTOR, "A class [{}] can have one constructor" },
    { Error::PARSE_ERROR, "Parsing error" },
    { Error::TESTS_FAILED, "Tests failed" },
    { Error::CIRCULAR_REFERENCE, "Circular reference: [{}]" },
    { Error::ERROR_CONST_MODIFIER, "Const modifier should be declare as [Type:const]: {}" },
    { Error::WARNING_TEST_CLASS_NOT_IMPLEMENT_METHOD, "Test class [{}] has not implemented method [{}]" },
    { Error::ERROR_VIRTUAL_METHOD_HAS_DIFFERENT_DECLARATION, "Class [{}] has different declaration of method [{}]. See class [{}]" },
    { Error::ERROR_SEMICOLON_IN_FUNCTION, "Syntax error: Found symbol \";\" in function declaration: [{}]\nIn line: \n{}" },
    { Error::ERROR_METHOD_HAS_NOT_BODY, "Error: method [{}] has not body.\nIn line: \n{}" },
    { Error::ERROR_SYNTAX_ERROR, "Error syntax: Line: \n{}" },
    
};

void Error::exit(Code code) {
    printAndExit(true, code, {});
}

void Error::printAndExit(bool fatal, Code code, const std::vector<std::string>& args) {
    auto it = kTexts.find(code);
    std::string pattern = (it != kTexts.end() ? it->second : std::string("Unknown error"));
    std::string text = format(pattern, args);
    std::string prefix = (fatal ? " Error " : " Warning ");
    std::string full = prefix + std::to_string(static_cast<int>(code)) + ": " + text;
    if (fatal) {
        Log::error(full);
        std::exit(static_cast<int>(code));
    } else {
        Log::warning(full);
    }
}

std::string Error::format(const std::string& pattern, const std::vector<std::string>& args) {
    std::string out = pattern;
    size_t pos = 0;
    for (const auto& a : args) {
        pos = out.find("{}", pos);
        if (pos == std::string::npos) break;
        out.replace(pos, 2, a);
        pos += a.size();
    }
    return out;
}

