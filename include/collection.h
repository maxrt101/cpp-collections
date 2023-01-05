#ifndef _MRT_COLLECTIONS_COLLECTION_H_
#define _MRT_COLLECTIONS_COLLECTION_H_ 1

#include <functional>
#include <concepts>

namespace mrt {

template <typename C, typename T>
concept Collection = requires (C c) {
  { c.size() } -> std::same_as<size_t>;
  // TODO: expand
};

template <typename C, typename T>
concept FilterableCollection = requires (C c, std::function<bool(const T&)> predicate) {
  { c.filter(predicate) } -> std::same_as<C>;
};

template <typename C, typename T, typename R>
concept ReducableCollection = requires (C c, std::function<R(R, const T&)> reducer) {
  { c.reduce(reducer) } -> std::same_as<R>;
};


} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_COLLECTION_H_ */