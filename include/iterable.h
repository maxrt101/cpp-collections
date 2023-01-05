#ifndef _MRT_COLLECTIONS_ITERABLE_H_
#define _MRT_COLLECTIONS_ITERABLE_H_ 1

#include <concepts>
#include <functional>
#include <mrt/utils/concepts.h>
#include <mrt/iterator.h>

namespace mrt {

template <typename C, typename T>
concept Iterable = requires (C c, std::function<void(const T&)> f) {
  { c.begin() } -> Iterator<T>;
  { c.end() } -> Iterator<T>;
  { c.cbegin() } -> ConstIterator<T>;
  { c.cend() } -> ConstIterator<T>;
  c.foreach(f);
};

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_ITERABLE_H_ */