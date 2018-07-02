// nanorange/algorithm/count.hpp
//
// Copyright (c) 2018 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NANORANGE_ALGORITHM_COUNT_HPP_INCLUDED
#define NANORANGE_ALGORITHM_COUNT_HPP_INCLUDED

#include <nanorange/range.hpp>

NANO_BEGIN_NAMESPACE

// [rng.alg.count]

namespace detail {

struct count_if_fn {
private:
    friend struct count_fn;
    friend struct is_permutation_fn;

    template <typename I, typename S, typename Proj, typename Pred>
    static constexpr iter_difference_t<I> impl(I first, S last, Pred& pred,
                                               Proj& proj)
    {
        iter_difference_t<I> counter = 0;

        for (; first != last; ++first) {
            if (nano::invoke(pred, nano::invoke(proj, *first))) {
                ++counter;
            }
        }

        return counter;
    }

public:
    template <typename I, typename S, typename Proj = identity, typename Pred>
    constexpr std::enable_if_t<
        InputIterator<I> && Sentinel<S, I> &&
            IndirectUnaryPredicate<Pred, projected<I, Proj>>,
        iter_difference_t<I>>
    operator()(I first, S last, Pred pred, Proj proj = Proj{}) const
    {
        return count_if_fn::impl(std::move(first), std::move(last),
                                 pred, proj);
    }

    template <typename Rng, typename Proj = identity, typename Pred>
    constexpr std::enable_if_t<
        InputRange<Rng> &&
            IndirectUnaryPredicate<Pred, projected<iterator_t<Rng>, Proj>>,
        iter_difference_t<iterator_t<Rng>>>
    operator()(Rng&& rng, Pred pred, Proj proj = Proj{}) const
    {
        return count_if_fn::impl(nano::begin(rng), nano::end(rng),
                                 pred, proj);
    }
};
} // namespace detail

NANO_DEFINE_CPO(detail::count_if_fn, count_if)

namespace detail {

struct count_fn {
private:
    template <typename ValueType>
    struct equal_to_pred {
        const ValueType& val;

        template <typename T>
        constexpr bool operator()(const T& t) const
        {
            return t == val;
        }
    };

public:
    template <typename I, typename S, typename T, typename Proj = identity>
    constexpr std::enable_if_t<
        InputIterator<I> && Sentinel<S, I> &&
            IndirectRelation<equal_to<>, projected<I, Proj>, const T*>,
        iter_difference_t<I>>
    operator()(I first, S last, const T& value, Proj proj = Proj{}) const
    {
        const auto pred = equal_to_pred<T>{value};
        return count_if_fn::impl(std::move(first), std::move(last),
                                 pred, proj);
    }

    template <typename Rng, typename T, typename Proj = identity>
    constexpr std::enable_if_t<
        InputRange<Rng> &&
            IndirectRelation<equal_to<>, projected<iterator_t<Rng>, Proj>,
                             const T*>,
        iter_difference_t<iterator_t<Rng>>>
    operator()(Rng&& rng, const T& value, Proj proj = Proj{}) const
    {
        const auto pred = equal_to_pred<T>{value};
        return count_if_fn::impl(nano::begin(rng), nano::end(rng),
                                 pred, proj);
    }
};

} // namespace detail

NANO_DEFINE_CPO(detail::count_fn, count)

NANO_END_NAMESPACE

#endif
