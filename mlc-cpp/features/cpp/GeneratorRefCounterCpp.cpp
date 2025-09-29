//
//  GeneratorRefCounterCpp.cpp
//  mlc-cpp
//
//  Created by Vladimir Tolmachev on 30.07.2025.
//

#include "GeneratorRefCounterCpp.hpp"
#include <sstream>

#include "../models/Model.hpp"
#include "../models/Class.hpp"
#include "../models/Object.hpp"
#include "../models/Function.hpp"

static const std::string RETAIN_INT = "++this->_reference_counter;";
static const std::string RETAIN_ATOMIC =
    "_reference_counter.fetch_add(1, std::memory_order_relaxed);";

static const std::string RELEASE_INT = R"(
--this->_reference_counter;
auto counter = this->_reference_counter;
if(counter == 0)
{
    delete this;
}
return counter;
)";

static const std::string RELEASE_ATOMIC = R"(
auto ret = _reference_counter.fetch_sub(1, std::memory_order_acq_rel);
if(ret == 1)
{
    delete this;
}
return ret;
)";

GeneratorRefCounterCpp::GeneratorRefCounterCpp()
  : preferType_("int")
{
}

void GeneratorRefCounterCpp::generate(Model &model) {
    if (model.config.language != "cpp")
        return;
    if (!model.config.generate_ref_counter)
        return;

    if (model.config.side == Side::server)
    {
        preferType_ = "std::atomic<int>";
        retainBody_  = RETAIN_ATOMIC;
        releaseBody_ = RELEASE_ATOMIC;
    }
    else
    {
        retainBody_  = RETAIN_INT;
        releaseBody_ = RELEASE_INT;
    }

    for (auto &cls : model.classes)
    {
        if (cls->parent_class_name.empty() && cls->type != "enum" && !cls->is_abstract)
            addToClass(cls);
    }
}

void GeneratorRefCounterCpp::addToClass(const std::shared_ptr<Class> &cls) {
    if (!cls->has_member("_reference_counter"))
    {
        Object ref;
        ref.name = "_reference_counter";
        ref.type = preferType_;
        ref.value = "1";
        ref.is_runtime = true;
        ref.access = AccessSpecifier::m_private;
        cls->members.push_back(ref);
    }

    if (!cls->has_method("retain"))
    {
        Function fn;
        fn.name = "retain";
        fn.return_type = Objects::VOID;
        fn.body = retainBody_;
        cls->functions.push_back(std::move(fn));
    }

    if (!cls->has_method("release"))
    {
        Function fn;
        fn.name = "release";
        fn.return_type = Objects::INT;
        fn.body = releaseBody_;
        cls->functions.push_back(std::move(fn));
    }
}

