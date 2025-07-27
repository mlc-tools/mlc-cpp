#include "parser.hpp"
#include <iostream>
#include <fstream>


std::string getFileContent(const std::string& path)
{
    std::fstream stream(path, std::ios::in);
    if (stream.is_open() == false)
        return "";
    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return str;
}

int main() {
    std::string code = getFileContent("/Users/vladimirtolmachev/work/archero/configs/ecs/systems/system_battle.mlc");

    std::vector<Class> classes = parse_class(code);
    auto cls = classes.at(1);

    std::cout << "Group: " << cls.group << "\n";
    std::cout << "Class: " << cls.name << "\n";
    std::cout << "Includes:\n";
    for (auto &i : cls.includes) std::cout << "  " << i << "\n";
    std::cout << "Members:\n";
    for (auto &m : cls.members) std::cout << "  " << m.type << " " << m.name << "\n";
    std::cout << "Methods:\n";
    for (auto &m : cls.methods) {
        std::cout << "  " << m.return_type.type << " " << m.name << "(";
        for (auto &a : m.args) std::cout << a.type << " " << a.name << " ";
        std::cout << ")\n";
    }
    std::cout << "Constructors:\n";
    for (auto &m : cls.constructors) {
        std::cout << "  " << m.name << "(";
        for (auto &a : m.args) std::cout << a.type << " " << a.name << " ";
        std::cout << ")\n";
    }
}
