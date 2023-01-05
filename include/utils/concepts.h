#ifndef _MRT_COLLECTIONS_UTILS_CONCEPTS_H_
#define _MRT_COLLECTIONS_UTILS_CONCEPTS_H_ 1

#include <concepts>
#include <cstdlib>

namespace mrt {

template <typename T>
concept Incrementable = requires (T t) {
  ++t; t++;
};

template <typename T>
concept Decrementable = requires (T t) {
  --t; t--;
};

template <typename T>
concept Comparable = requires (T t1, T t2) {
  { t1 == t2 } -> std::same_as<bool>;
  { t1 != t2 } -> std::same_as<bool>;
};

template <typename C>
concept Dereferencable = requires (C c) {
  *c;
};

template <typename C, typename T>
concept DereferencableIntoT = requires (C c) {
  { *c } -> std::convertible_to<T>;
};

template <typename C, typename K, typename V>
concept Subscriptable = requires (C c, K k) {
  { c[k] } -> std::same_as<V>;
};

template <typename C, typename T>
concept Indexable = Subscriptable<C, size_t, T>;

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_UTILS_CONCEPTS_H_ */