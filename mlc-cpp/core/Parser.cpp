//
//  Parser.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 27.07.2025.
//

#include "Parser.hpp"
#include "ParserLexem.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

Parser::Parser(Model &m) : _model(m) {}

namespace {
// Parameter names, defaults and return types do not distinguish overloads.
std::string argumentType(const Object &arg, const Function &fn) {
    std::string type = arg.type;
    for (size_t i = 0; i < fn.template_args.size(); ++i) {
        if (type == fn.template_args[i].type) {
            type = "$" + std::to_string(i);
            break;
        }
    }
    if (arg.is_pointer || arg.is_raw_pointer || arg.is_link) type += "*";
    if (arg.is_ref) type += "&";
    if (arg.is_rvalue) type += "&";
    if ((arg.is_const || arg.is_link) &&
        (arg.is_pointer || arg.is_raw_pointer || arg.is_link || arg.is_ref))
        type += " const";
    for (auto &item : arg.template_args)
        type += "<" + argumentType(item, fn) + ">";
    for (auto &item : arg.callable_args)
        type += "(" + argumentType(item, fn) + ")";
    return type;
}

bool sameSignature(const Function &a, const Function &b) {
    if (a.name != b.name || a.template_args.size() != b.template_args.size() ||
        a.callable_args.size() != b.callable_args.size() ||
        (!a.is_static && !b.is_static && a.is_const != b.is_const))
        return false;
    for (size_t i = 0; i < a.callable_args.size(); ++i)
        if (argumentType(a.callable_args[i], a) != argumentType(b.callable_args[i], b))
            return false;
    return true;
}
}

void Parser::addClass(const std::shared_ptr<Class> &cls) {
    auto existing = _model.get_class(cls->name);
    if (!existing) {
        _model.add_classes({cls});
        return;
    }
    auto conflict = [&](const std::string &detail) {
        Error::exit(Error::CLASS_DECLARATION_CONFLICT, cls->name, detail,
                    existing->source_path, cls->source_path);
    };
    if (existing->type != cls->type)
        conflict("class kind");
    if (existing->group != cls->group)
        conflict("group: " + existing->group + " / " + cls->group);
    if (!existing->parent_class_name.empty() && !cls->parent_class_name.empty() &&
        existing->parent_class_name != cls->parent_class_name)
        conflict("parent: " + existing->parent_class_name + " / " + cls->parent_class_name);
    if (existing->side != Side::both && cls->side != Side::both && existing->side != cls->side)
        conflict("side");

    for (auto &member : cls->members) {
        const auto &name = member.name.empty() ? member.type : member.name;
        for (auto &old : existing->members)
            if ((old.name.empty() ? old.type : old.name) == name)
                conflict("member: " + name);
        if (existing->has_method(name))
            conflict("member/method: " + name);
    }
    for (auto &fn : cls->functions) {
        if (existing->has_member(fn.name))
            conflict("member/method: " + fn.name);
        for (auto &old : existing->functions)
            if (sameSignature(old, fn))
                conflict("method: " + fn.name);
    }
    auto constructorSignature = [&](const Function &ctor) {
        Class combined;
        combined.members = existing->members;
        combined.members.insert(combined.members.end(), cls->members.begin(), cls->members.end());
        combined.constructors.push_back(ctor);
        combined.generate_constructor();
        return combined.constructors.front();
    };
    for (auto &ctor : cls->constructors)
        for (auto &old : existing->constructors)
            if (sameSignature(constructorSignature(old), constructorSignature(ctor)))
                conflict("constructor: " + ctor.name);

    if (existing->parent_class_name.empty())
        existing->parent_class_name = cls->parent_class_name;
    existing->members.insert(existing->members.end(), cls->members.begin(), cls->members.end());
    existing->functions.insert(existing->functions.end(), cls->functions.begin(), cls->functions.end());
    existing->constructors.insert(existing->constructors.end(), cls->constructors.begin(), cls->constructors.end());
    for (auto &inner : cls->inner_classes) {
        auto old = std::find_if(existing->inner_classes.begin(), existing->inner_classes.end(),
                               [&](const auto &item) { return item->name == inner->name; });
        if (old == existing->inner_classes.end())
            existing->inner_classes.push_back(inner);
    }
    existing->user_includes.insert(cls->user_includes.begin(), cls->user_includes.end());
    existing->inner_body += "\n" + cls->inner_body;
    existing->is_abstract |= cls->is_abstract;
    existing->is_storage |= cls->is_storage;
    existing->is_visitor |= cls->is_visitor;
    existing->is_numeric |= cls->is_numeric;
    existing->is_test |= cls->is_test;
    existing->is_inline |= cls->is_inline;
    existing->is_virtual |= cls->is_virtual;
    existing->discard_virtual |= cls->discard_virtual;
    existing->prefer_use_forward_declarations |= cls->prefer_use_forward_declarations;
    existing->has_bindings |= cls->has_bindings;
    existing->discard_copy_ctr |= cls->discard_copy_ctr;
    existing->discard_copy |= cls->discard_copy;
    existing->discard_move |= cls->discard_move;
    if (existing->side == Side::both)
        existing->side = cls->side;
    existing->lang_specific.insert(cls->lang_specific.begin(), cls->lang_specific.end());
    if (!cls->source_path.empty())
        _model.source_to_classnames[cls->source_path].push_back(cls->name);
}

void Parser::parseFiles(const std::vector<std::string> &filePaths) {
    for (const auto &path : filePaths) {
        std::ifstream ifs(path);
        std::stringstream buf;
        buf << ifs.rdbuf();
        _current_source_path = path;
        parseText(buf.str());
    }
    std::set<std::string> parsedClasses;
    for (auto &path : filePaths)
        for (auto &name : _model.source_to_classnames[path])
            if (parsedClasses.insert(name).second)
                _model.get_class(name)->generate_constructor();
}

void Parser::parseText(const std::string &input) {
    std::string text = removeComments(input);
    ParserLexem parser(text, _model);
    auto classes = parser.parse_dict();

    std::vector<std::shared_ptr<Class>> inner_classes;
    for (auto &cls : classes) {
        parse_class(cls);
        for (auto &inner : cls->inner_classes) {
            for (auto &m : cls->members) {
                if (m.type == inner->name)
                    m.type = cls->name + inner->name;
                for (auto &t : m.template_args) {
                    if (t.type == inner->name)
                        t.type = cls->name + inner->name;
                }
                for (auto &t : m.callable_args) {
                    if (t.type == inner->name)
                        t.type = cls->name + inner->name;
                }
            }
            for (auto &m : cls->functions) {
                if (m.return_type.type == inner->name)
                    m.return_type.type = cls->name + inner->name;
                for (auto &t : m.template_args) {
                    if (t.type == inner->name)
                        t.type = cls->name + inner->name;
                }
                for (auto &t : m.callable_args) {
                    if (t.type == inner->name)
                        t.type = cls->name + inner->name;
                }
            }
            inner->name = cls->name + inner->name;
            inner->group = cls->group;
            if(inner->side == Side::both)
                inner->side = cls->side;
        }
        inner_classes.insert(inner_classes.end(), cls->inner_classes.begin(),
                             cls->inner_classes.end());
    }

    for (auto &cls : inner_classes) {
        parse_class(cls);
        assert(cls->inner_classes.empty());
    }

    // проставим исходный файл
    for (auto &c : classes)
        c->source_path = _current_source_path;
    for (auto &c : inner_classes)
        c->source_path = _current_source_path;
    for (auto &cls : classes)
        addClass(cls);
    for (auto &cls : inner_classes)
        addClass(cls);
}

void Parser::parse_class(const std::shared_ptr<Class> &cls) {
    ParserLexem parser(cls->inner_body, _model);
    parser.parse(cls);
}

std::string Parser::removeComments(const std::string &txt) {
    std::string s = txt;
    // remove /* */
    size_t p;
    while ((p = s.find("/*")) != std::string::npos) {
        size_t q = s.find("*/", p + 2);
        if (q != std::string::npos)
            s.erase(p, q - p + 2);
        else
            break;
    }
    // remove //
    std::istringstream iss(s);
    std::string line, out;
    while (std::getline(iss, line)) {
        auto cpos = line.find("//");
        if (cpos != std::string::npos)
            line.erase(cpos);
        out += line + '\n';
    }
    return out;
}
