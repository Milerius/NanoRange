// nanorange/algorithm/find_first_of.hpp
//
// Copyright (c) 2018 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NANORANGE_ALGORITHM_FIND_FIRST_OF_HPP_INCLUDED
#define NANORANGE_ALGORITHM_FIND_FIRST_OF_HPP_INCLUDED

#include <nanorange/range.hpp>

NANO_BEGIN_NAMESPACE

// [range.alg.find.first.of]

namespace detail {

struct find_first_of_fn {
private:
    template <typename I1, typename S1, typename I2, typename S2, typename Pred,
              typename Proj1, typename Proj2>
    static constexpr I1 impl(I1 first1, S1 last1, I2 first2, S2 last2,
                             Pred pred, Proj1 proj1, Proj2 proj2)
    {
        for (; first1 != last1; ++first1) {
            for (I2 it = first2; it != last2; ++it) {
                if (nano::invoke(pred, nano::invoke(proj1, *first1),
                                 nano::invoke(proj2, *it))) {
                    return first1;
                }
            }
        }

        return first1;
    }

public:
    template <typename I1, typename S1, typename I2, typename S2,
              typename Proj1 = identity, typename Proj2 = identity,
              typename Pred = equal_to<>>
    constexpr std::enable_if_t<
        InputIterator<I1> && Sentinel<S1, I1> && ForwardIterator<I2> &&
            Sentinel<S2, I2> &&
            IndirectRelation<Pred, projected<I1, Proj1>, projected<I2, Proj2>>,
        I1>
    operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = Pred{},
               Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        return find_first_of_fn::impl(std::move(first1), std::move(last1),
                                      std::move(first2), std::move(last2),
                                      std::move(pred), std::move(proj1),
                                      std::move(proj2));
    }

    template <typename Rng1, typename Rng2, typename Proj1 = identity,
              typename Proj2 = identity, typename Pred = equal_to<>>
    constexpr std::enable_if_t<
        InputRange<Rng1> && ForwardRange<Rng2> &&
            IndirectRelation<Pred, projected<iterator_t<Rng1>, Proj1>,
                             projected<iterator_t<Rng2>, Proj2>>,
        safe_iterator_t<Rng1>>
    operator()(Rng1&& rng1, Rng2&& rng2, Pred pred = Pred{},
               Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        return find_first_of_fn::impl(nano::begin(rng1), nano::end(rng1),
                                      nano::begin(rng2), nano::end(rng2),
                                      std::move(pred), std::move(proj1),
                                      std::move(proj2));
    }
};

} // namespace detail

NANO_DEFINE_CPO(detail::find_first_of_fn, find_first_of)

NANO_END_NAMESPACE

#endif