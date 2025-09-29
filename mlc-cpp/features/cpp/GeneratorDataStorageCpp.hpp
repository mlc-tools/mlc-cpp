//
//  GeneratorDataStorageCpp.hpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#ifndef GeneratorDataStorageCpp_hpp
#define GeneratorDataStorageCpp_hpp

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "features/GeneratorDataStorageBase.hpp"
#include "features/FeatureGenerator.hpp"


class GeneratorDataStorageCpp : public FeatureGenerator, public GeneratorDataStorageBase {
public:
    GeneratorDataStorageCpp();

    virtual void generate(Model &model) override;
    virtual void modifySources(Model& model, const std::shared_ptr<Class>& cls, std::string& header, std::string& source) override{}

protected:
    // Не создаём статический instance в C++
    virtual bool isNeedCreateStaticInstance() const override;

    // Тело метода shared()
    virtual std::string getSharedMethodBody() override;

    // Вместо базового createGetters(), делаем свою версию, возвращающую
    // созданный getter-метод, чтобы потом в него добавить specific implementations.
    // Этот метод НЕ override — он скрывает базовый.
    virtual void createGetters(const std::vector<std::shared_ptr<Class>>& classes) override;
    virtual std::string getPatternGetter() override { return ""; };

    // Реализует все специализации get< T > для каждого storage-класса
    virtual void generateImplementations(const std::vector<std::shared_ptr<Class>>& classes,
                                 Function &getter);

    // Возвращает все storage-классы, релевантные для данной модели
    virtual std::vector<std::shared_ptr<Class>> getStorageClasses() const;

    // Тела функций инициализации, специфичные для JSON и XML
    virtual std::string getInitializeFunctionJsonBody() override;
    virtual std::string getInitializeFunctionXmlBody()  override;
};


#endif /* GeneratorDataStorageCpp_hpp */
