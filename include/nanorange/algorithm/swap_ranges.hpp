// nanorange/algorithm/swap_ranges.hpp
//
// Copyright (c) 2018 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NANORANGE_ALGORITHM_SWAP_RANGES_HPP_INCLUDED
#define NANORANGE_ALGORITHM_SWAP_RANGES_HPP_INCLUDED

#include <nanorange/range.hpp>

NANO_BEGIN_NAMESPACE

namespace detail {

// FIXME: Use tagged pair
struct swap_ranges_fn {
    template <typename I1, typename S1, typename I2, typename S2>
    static constexpr std::pair<I1, I2> impl4(I1 first1 ,S1 last1, I2 first2, S2 last2)
    {
        while (first1 != last1 && first2 != last2) {
            nano::iter_swap(first1, first2);
            ++first1; ++first2;
        }
        return {std::move(first1), std::move(first2)};
    }

    template <typename I1, typename S1, typename I2>
    static constexpr std::pair<I1, I2> impl3(I1 first1, S1 last1, I2 first2)
    {
        while (first1 != last1) {
            nano::iter_swap(first1, first2);
            ++first1; ++first2;
        }
        return {std::move(first1), std::move(first2)};
    }

public:
    template <typename I1, typename S1, typename I2, typename S2>
    constexpr std::enable_if_t<
        ForwardIterator<I1> &&
        Sentinel<S1, I1> &&
        ForwardIterator<I2> &&
        Sentinel<S2, I2> &&
        IndirectlySwappable<I1, I2>,
        std::pair<I1, I2>>
    operator()(I1 first1 ,S1 last1, I2 first2, S2 last2) const
    {
        return swap_ranges_fn::impl4(std::move(first1), std::move(last1),
                                     std::move(first2), std::move(last2));
    }

    template <typename I1, typename S1, typename I2>
    NANO_DEPRECATED
    constexpr std::enable_if_t<
            ForwardIterator<I1> &&
            Sentinel<S1, I1> &&
            ForwardIterator<I2> &&
            IndirectlySwappable<I1, I2>,
            std::pair<I1, I2>>
    operator()(I1 first1 ,S1 last1, I2 first2) const
    {
        return swap_ranges_fn::impl3(std::move(first1), std::move(last1),
                                     std::move(first2));
    }

    template <typename Rng1, typename Rng2>
    constexpr std::enable_if_t<
            ForwardRange<Rng1> &&
            ForwardRange<Rng2> &&
            IndirectlySwappable<iterator_t<Rng1>, iterator_t<Rng2>>,
            std::pair<safe_iterator_t<Rng1>, safe_iterator_t<Rng2>>>
    operator()(Rng1&& rng1, Rng2&& rng2) const
    {
        return swap_ranges_fn::impl4(nano::begin(rng1), nano::end(rng1),
                                     nano::begin(rng2), nano::end(rng2));
    }

    template <typename Rng1, typename I2>
    NANO_DEPRECATED
    constexpr std::enable_if_t<
            ForwardRange<Rng1> &&
            ForwardIterator<I2> &&
            IndirectlySwappable<iterator_t<Rng1>, I2>,
            std::pair<safe_iterator_t<Rng1>, I2>>
    operator()(Rng1&& rng1, I2 first2) const
    {
        return swap_ranges_fn::impl3(nano::begin(rng1), nano::end(rng1),
                                     std::move(first2));
    }
};

}

NANO_DEFINE_CPO(detail::swap_ranges_fn, swap_ranges)

NANO_END_NAMESPACE

#endif
