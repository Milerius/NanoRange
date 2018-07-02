// nanorange/algorithm/remove_copy_if.hpp
//
// Copyright (c) 2018 Tristan Brindle (tcbrindle at gmail dot com)
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef NANORANGE_ALGORITHM_REMOVE_COPY_IF_HPP_INCLUDED
#define NANORANGE_ALGORITHM_REMOVE_COPY_IF_HPP_INCLUDED

#include <nanorange/range.hpp>

#include <nanorange/algorithm/find.hpp>

NANO_BEGIN_NAMESPACE

namespace detail {

// FIXME: Use tagged_pair
struct remove_copy_if_fn {
private:
    template <typename I, typename S, typename O, typename Pred, typename Proj>
    static constexpr std::pair<I, O> impl(I first, S last, O result,
                                          Pred& pred, Proj& proj)
    {
        while (first != last) {
            auto&& ref = *first;
            if (!nano::invoke(pred, nano::invoke(proj, ref))) {
                *result = std::forward<decltype(ref)>(ref);
                ++result;
            }
            ++first;
        }
        return {std::move(first), std::move(result)};
    }

public:
    template <typename I, typename S, typename O, typename Pred,
              typename Proj = identity>
    constexpr std::enable_if_t<
        InputIterator<I> && Sentinel<S, I> && WeaklyIncrementable<O> &&
            IndirectlyCopyable<I, O> &&
            IndirectUnaryPredicate<Pred, projected<I, Proj>>,
        std::pair<I, O>>
    operator()(I first, S last, O result, Pred pred,
               Proj proj = Proj{}) const
    {
        return remove_copy_if_fn::impl(std::move(first), std::move(last),
                                    std::move(result), pred, proj);
    }

    template <typename Rng, typename O, typename Pred, typename Proj = identity>
    constexpr std::enable_if_t<
        InputRange<Rng> && WeaklyIncrementable<O> &&
            IndirectlyCopyable<iterator_t<Rng>, O> &&
            IndirectUnaryPredicate<Pred, projected<iterator_t<Rng>, Proj>>,
        std::pair<safe_iterator_t<Rng>, O>>
    operator()(Rng&& rng, O result, Pred pred, Proj proj = Proj{}) const
    {
        return remove_copy_if_fn::impl(nano::begin(rng), nano::end(rng),
                                       std::move(result), pred, proj);
    }
};

} // namespace detail

NANO_DEFINE_CPO(detail::remove_copy_if_fn, remove_copy_if)

NANO_END_NAMESPACE

#endif
