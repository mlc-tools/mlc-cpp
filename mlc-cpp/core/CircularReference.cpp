//
//  CircularReference.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 28.07.2025.
//

#include "CircularReference.hpp"
#include "Error.hpp"
#include "Model.hpp"
#include <sstream>

CircularReference::CircularReference(Model &model) : _model(model) {}

void CircularReference::find() {
    // Для каждого класса в модели запускаем «волновой» обход
    for (const auto &cls : _model.classes) {
        viewed_.clear();
        wave(cls, cls);
    }

    // Если что-то накопилось — это цикл
    if (!circularReference_.empty()) {
        // Замыкаем круг, добавив в конец имя первого узла
        circularReference_.push_back(circularReference_.front());

        // Формируем строку вида A->B->C->A
        std::string s;
        for (size_t i = 0; i < circularReference_.size(); ++i) {
            if (i)
                s += "->";
            s += circularReference_[i];
        }
        Error::exit(Error::CIRCULAR_REFERENCE, s);
    }
}

void CircularReference::wave(const std::shared_ptr<Class> &baseClass,
                             const std::shared_ptr<Class> &rootClass) {
    const std::string &rootName = rootClass->name;

    // Уже посещали — выходим
    if (viewed_.count(rootName))
        return;
    viewed_.insert(rootName);

    // Проходим по всем членам rootClass
    for (const auto &member : rootClass->members) {
        // Если тип члена совпал с именем базового класса,
        // и это не та же самая вершина — фиксируем цикл
        if (member.type == baseClass->name && rootName != baseClass->name) {
            circularReference_.push_back(rootName);
        }
        // Если модель знает такой класс — спускаемся глубже
        if (_model.hasClass(member.type)) {
            auto nextCls = _model.get_class(member.type);
            wave(baseClass, nextCls);
        }
    }
}
