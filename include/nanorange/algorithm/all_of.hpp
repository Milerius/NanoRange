// nanorange/algorithm/all_of.hpp
//
// Copyright (c) 2018 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NANORANGE_ALGORITHM_ALL_OF_HPP_INCLUDED
#define NANORANGE_ALGORITHM_ALL_OF_HPP_INCLUDED

#include <nanorange/range.hpp>

NANO_BEGIN_NAMESPACE

// [range.alg.all_of]

namespace detail {

struct all_of_fn {
private:
    template <typename I, typename S, typename Proj, typename Pred>
    static constexpr bool impl(I first, S last, Pred pred, Proj proj)
    {
        while (first != last) {
            if (!nano::invoke(pred, nano::invoke(proj, *first))) {
                return false;
            }
            ++first;
        }
        return true;
    }

public:
    template <typename I, typename S, typename Proj = identity, typename Pred>
    constexpr std::enable_if_t<
        InputIterator<I> && Sentinel<S, I> &&
            IndirectUnaryPredicate<Pred, projected<I, Proj>>,
        bool>
    operator()(I first, S last, Pred pred, Proj proj = Proj{}) const
    {
        return all_of_fn::impl(std::move(first), std::move(last),
                               std::move(pred), std::move(proj));
    }

    template <typename Rng, typename Proj = identity, typename Pred>
    constexpr std::enable_if_t<
        InputRange<Rng> &&
            IndirectUnaryPredicate<Pred, projected<iterator_t<Rng>, Proj>>,
        bool>
    operator()(Rng&& rng, Pred pred, Proj proj = Proj{}) const
    {
        return all_of_fn::impl(nano::begin(rng), nano::end(rng),
                               std::move(pred), std::move(proj));
    }
};

} // namespace detail

NANO_DEFINE_CPO(detail::all_of_fn, all_of)

NANO_END_NAMESPACE

#endif
