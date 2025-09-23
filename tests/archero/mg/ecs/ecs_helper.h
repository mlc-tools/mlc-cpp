#ifndef __mg_ecs_helper_h__
#define __mg_ecs_helper_h__

#include <vector>
#include <tuple>
#include "intrusive_ptr.h"
#include <algorithm>

template<typename A, typename B>
auto join_two_sorted(
    const std::vector<intrusive_ptr<A>>& va,
    const std::vector<intrusive_ptr<B>>& vb)
{
    std::vector<std::pair<intrusive_ptr<A>, intrusive_ptr<B>>> result;
    result.reserve(std::min(va.size(), vb.size()));

    size_t ia = 0, ib = 0;
    while (ia < va.size() && ib < vb.size()) {
        int idA = va[ia]->id;
        int idB = vb[ib]->id;
        if      (idA < idB) ++ia;
        else if (idB < idA) ++ib;
        else {
            result.emplace_back(va[ia], vb[ib]);
            ++ia; ++ib;
        }
    }
    return result;
}

template<typename Tuple, typename C>
auto join_tuple_vector(
    const std::vector<Tuple>& partial,
    const std::vector<intrusive_ptr<C>>& vc)
{
    using new_tuple_t = decltype(
        std::tuple_cat(
            std::declval<Tuple>(),
            std::declval<std::tuple<intrusive_ptr<C>>>()
        )
    );

    std::vector<new_tuple_t> result;
    result.reserve(std::min(partial.size(), vc.size()));

    size_t i = 0, j = 0;
    while (i < partial.size() && j < vc.size()) {
        int idP = std::get<0>(partial[i])->id;
        int idC = vc[j]->id;
        if      (idP < idC) ++i;
        else if (idC < idP) ++j;
        else {
            result.emplace_back(
                std::tuple_cat(partial[i], std::make_tuple(vc[j]))
            );
            ++i; ++j;
        }
    }
    return result;
}

template<typename A, typename B>
auto join_all(
    const std::vector<intrusive_ptr<A>>& va,
    const std::vector<intrusive_ptr<B>>& vb)
{
    return join_two_sorted(va, vb);
}

template<typename Tuple, typename C, typename... Rest>
auto join_all(
    const std::vector<Tuple>&                  partial,
    const std::vector<intrusive_ptr<C>>&     vc,
    const std::vector<intrusive_ptr<Rest>>&... rest)
{
    auto next = join_tuple_vector(partial, vc);
    if constexpr(sizeof...(rest) > 0) {
        return join_all(next, rest...);
    }
    else {
        return next;
    }
}
#endif
