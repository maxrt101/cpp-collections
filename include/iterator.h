#ifndef _MRT_COLLECTIONS_ITERATOR_H_
#define _MRT_COLLECTIONS_ITERATOR_H_ 1

#include <concepts>
#include <mrt/utils/concepts.h>

namespace mrt {

template <typename I, typename T>
concept ForwardIterator =
  Incrementable<I> && Comparable<I> && DereferencableIntoT<I, T>;

template <typename I, typename T>
concept BackwardIterator =
  Decrementable<I> && Comparable<I> && DereferencableIntoT<I, T>;

template <typename I, typename T>
concept BidirectionalIterator =
  Incrementable<I> && Decrementable<I> && Comparable<I> && DereferencableIntoT<I, T>;

template <typename I, typename T>
concept Iterator = ForwardIterator<I, T> || BackwardIterator<I, T> || BidirectionalIterator<I, T>;


template <typename I, typename T>
concept ConstForwardIterator =
  Incrementable<I> && Comparable<I> && DereferencableIntoT<I, const T>;

template <typename I, typename T>
concept ConstBackwardIterator =
  Decrementable<I> && Comparable<I> && DereferencableIntoT<I, const T>;

template <typename I, typename T>
concept ConstBidirectionalIterator =
  Incrementable<I> && Decrementable<I> && Comparable<I> && DereferencableIntoT<I, const T>;

template <typename I, typename T>
concept ConstIterator = ConstForwardIterator<I, T> || ConstBackwardIterator<I, T> || ConstBidirectionalIterator<I, T>;

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_ITERATOR_H_ */