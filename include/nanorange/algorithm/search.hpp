// nanorange/algorithm/search.hpp
//
// Copyright (c) 2018 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NANORANGE_ALGORITHM_SEARCH_HPP_INCLUDED
#define NANORANGE_ALGORITHM_SEARCH_HPP_INCLUDED

#include <nanorange/range.hpp>

NANO_BEGIN_NAMESPACE

namespace detail {

struct search_fn {
private:
    friend struct find_end_fn;

    template <typename I1, typename S1, typename I2, typename S2,
              typename Pred, typename Proj1, typename Proj2 = identity>
    static constexpr I1 impl(I1 first1, S1 last1, I2 first2, S2 last2,
                             Pred pred, Proj1 proj1, Proj2 proj2 = Proj2{})
    {
        while (true) {
            auto it1 = first1;
            auto it2 = first2;

            while (true) {
                if (it2 == last2) {
                    return first1;
                }
                if (it1 == last1) {
                    return it1;
                }
                if (!nano::invoke(pred, nano::invoke(proj1, *it1), nano::invoke(proj2, *it2))) {
                    break;
                }
                ++it1; ++it2;
            }


            ++first1;
        }
    }

public:
    template <typename I1, typename S1, typename I2, typename S2,
              typename Pred = equal_to<>, typename Proj1 = identity,
              typename Proj2 = identity>
    constexpr std::enable_if_t<
         ForwardIterator<I1> &&
         Sentinel<S1, I1> &&
         ForwardIterator<I2> &&
         Sentinel<S2, I2> &&
         IndirectlyComparable<I1, I2, Pred, Proj1, Proj2>, I1>
    operator()(I1 first1, S1 last1, I2 first2, S2 last2,
               Pred pred = Pred{}, Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        return search_fn::impl(std::move(first1), std::move(last1),
                               std::move(first2), std::move(last2),
                               std::move(pred), std::move(proj1),
                               std::move(proj2));
    }

    template <typename Rng1, typename Rng2,
              typename Pred = equal_to<>, typename Proj1 = identity,
              typename Proj2 = identity>
    constexpr std::enable_if_t<
            ForwardRange<Rng1> &&
            ForwardRange<Rng2> &&
            IndirectlyComparable<iterator_t<Rng1>, iterator_t<Rng2>, Pred, Proj1, Proj2>,
            safe_iterator_t<Rng1>>
    operator()(Rng1&& rng1, Rng2&& rng2, Pred pred = Pred{},
               Proj1 proj1 = Proj1{}, Proj2 proj2 = Proj2{}) const
    {
        return search_fn::impl(nano::begin(rng1), nano::end(rng1),
                               nano::begin(rng2), nano::end(rng2),
                               std::move(pred), std::move(proj1),
                               std::move(proj2));
    }
};

}

NANO_DEFINE_CPO(detail::search_fn, search)

NANO_END_NAMESPACE

#endif