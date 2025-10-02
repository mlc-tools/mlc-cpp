//
//  WriterPython.hpp
//  mlc-cpp
//
//  Python code writer (ported from legacy Python writer)
//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../core/WriterBase.hpp"

class WriterPython : public WriterBase {
public:
    WriterPython();

    std::vector<std::pair<std::string, std::string>>
    writeClass(const std::shared_ptr<Class> &cls) override;

protected:
    std::tuple<std::string, std::string, std::string>
    writeObject(const Object &obj) override;
    std::pair<std::string, std::string>
    writeFunction(const Function &fn) override;
    std::string getMethodArgPattern(const Object &arg) const;
    std::string getMethodPattern(const Function &fn) const;
    std::string getRequiredArgs(const Function &method) const override;
    std::string getNullptrString() const override;
    std::string addStaticModifierToMethod(const std::string &text) const;
    virtual std::string
    convertInitializeValue(const std::string &value) override;

private:
    std::string preparePythonFile(const std::string &text) const;
};
