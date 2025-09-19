//
//  Error.hpp
//  mlc-cpp
//
//  Logging (with ANSI colors) and error reporting utilities.
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// Simple ANSI color holder
struct Color {
    static constexpr const char* red        = "\033[31m";
    static constexpr const char* green      = "\033[32m";
    static constexpr const char* orange     = "\033[33m";
    static constexpr const char* light_grey = "\033[37m";
    static constexpr const char* end        = "\033[0m";
};

struct Log {
    static bool use_colors;
    static bool disable_logs;

    static void debug(const std::string& msg);
    static void error(const std::string& msg);
    static void warning(const std::string& msg);
    static void message(const std::string& msg);
};

// Error codes and messages
struct Error {
    enum Code {
        UNKNOWN_SERIALISED_TYPE = 1001,
        STATIS_MEMBER_SHOULD_HAVE_INITIALISATION,
        MAP_TWO_ARGS,
        DUBLICATE_METHODS,
        DUBLICATE_CLASS,
        UNKNOWN_SUPERCLASS,
        UNKNOWN_CLASS,
        CANNOT_FIND_CLASS_FOR_METHOD,
        CANNOT_FIND_CLASS_FOR_OBJECT,
        OBJECT_IS_KEY_OF_MAP,
        CANNOT_PARSE_XML,
        ENUM_CANNOT_BE_COMBINATED,
        INTERNAL_ERROR,
        WARNING_SYNTAX,
        UNKNOWN_DATA_TYPE,
        CLASS_HAVE_MORE_THAN_ONE_CONSTRUCTOR,
        PARSE_ERROR,
        TESTS_FAILED,
        CIRCULAR_REFERENCE,
        ERROR_CONST_MODIFIER,
        WARNING_TEST_CLASS_NOT_IMPLEMENT_METHOD,
        ERROR_VIRTUAL_METHOD_HAS_DIFFERENT_DECLARATION,
    };

    static void exit(Code code);

    template <typename... Args>
    static void exit(Code code, Args&&... args) {
        printAndExit(true, code, toVector(std::forward<Args>(args)...));
    }

    template <typename... Args>
    static void warning(Code code, Args&&... args) {
        printAndExit(false, code, toVector(std::forward<Args>(args)...));
    }

private:
    static void printAndExit(bool fatal, Code code, const std::vector<std::string>& args);
    static std::string format(const std::string& pattern, const std::vector<std::string>& args);

    template <typename T>
    static std::string toStr(T&& v) {
        using U = std::decay_t<T>;
        if constexpr (std::is_same_v<U, std::string>) return v;
        else if constexpr (std::is_same_v<U, const char*>) return std::string(v);
        else return std::to_string(v);
    }
    template <typename... Args>
    static std::vector<std::string> toVector(Args&&... a) {
        return { toStr(std::forward<Args>(a))... };
    }
};

