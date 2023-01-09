#ifndef _MRT_COLLECTIONS_SORT_H_
#define _MRT_COLLECTIONS_SORT_H_ 1

#include <functional>
#include <concepts>
#include <mrt/collection.h>

namespace mrt {

template <typename T>
using SortComparator = std::function<bool(T&, T&)>;

template <typename S, typename T, typename C>
concept Sorter = Collection<C, T> and requires (S s, C& c, SortComparator<T> sc) {
  s.sort(sc, c);
};

template <typename T>
inline bool desc(T lhs, T rhs) {
  return lhs > rhs;
}

template <typename T>
inline bool asc(T lhs, T rhs) {
  return lhs < rhs;
}

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_SORT_H_ */