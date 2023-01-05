#ifndef _MRT_COLLECTIONS_PAIR_H_
#define _MRT_COLLECTIONS_PAIR_H_ 1

namespace mrt {

template <typename T1, typename T2>
struct Pair {
  T1 _1;
  T2 _2;

 public:
  inline Pair() {}
  inline Pair(const T1& _1) : _1(_1) {}
  inline Pair(const T1& _1, const T2& _2) : _1(_1), _2(_2) {}

  virtual inline ~Pair() {}
};

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_PAIR_H_ */