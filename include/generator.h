#ifndef _MRT_COLLECTIONS_GENERATOR_H_
#define _MRT_COLLECTIONS_GENERATOR_H_ 1

#include <concepts>
#include <exception>
#include <functional>
#include <mrt/utils/concepts.h>
#include <mrt/iterator.h>
#include <mrt/iterable.h>
#include <mrt/pair.h>

namespace mrt {

struct GeneratorException : public std::exception {
  inline GeneratorException() {}
};

template <typename T>
class Generator {
 public:
  using Function = std::function<T(size_t)>;

  class Iterator {
   public:
    inline Iterator() {}

    inline Iterator(Function fn) : m_function(fn) {
      operator++();
    }

    inline T operator*() {
      return m_lastValue;
    }

    inline const T operator*() const {
      return m_lastValue;
    }

    inline Iterator& operator++() {
      try {
        m_lastValue = m_function(m_counter++);
      } catch (GeneratorException e) {
        m_isEnded = true;
      }
      return *this;
    }

    inline Iterator operator++(int) {
      Iterator it = *this;
      try {
        m_lastValue = m_function(++m_counter);
      } catch (GeneratorException e) {
        m_isEnded = true;
      }
      return it;
    }

    inline bool operator==(const Iterator& rhs) const {
      return m_isEnded;
    }

    inline bool operator!=(const Iterator& rhs) const {
      return !m_isEnded;
    }

   private:
    Function m_function;
    size_t m_counter = 0;
    T m_lastValue;
    bool m_isEnded = false;
  };

 public:
  inline Generator(Function fn) : m_function(fn) {}

  inline Iterator begin() {
    return Iterator(m_function);
  }

  inline Iterator end() {
    return Iterator();
  }

  inline Iterator cbegin() const {
    return Iterator(m_function);
  }

  inline Iterator cend() const {
    return Iterator();
  }

  void foreach(std::function<void(const T&)> f) {
    for (auto iter : *this) {
      f(iter);
    }
  }

 private:
  Function m_function;
};

namespace {

template <typename T, Iterator<T> I>
struct Enumerator {
  I begin, end;

  inline Enumerator(I begin, I end) : begin(begin), end(end) {}

  inline Pair<size_t, T> operator()(size_t count) {
    if (begin == end) throw GeneratorException();
    return {count, *begin++};
  }
};

struct RangeGenerator {
  size_t begin, end, step, counter = 0;

  inline RangeGenerator(size_t begin, size_t end, size_t step) : begin(begin), end(end), step(step) {
    if (end == -1UL) {
      this->begin = 0;
      this->end = begin;
    }
  }
  
  inline size_t operator()(size_t count) {
    if (counter >= end) throw GeneratorException();
    size_t result = counter;
    counter += step;
    return result;
  }
};

template <typename T1, typename T2, Iterator<T1> I1, Iterator<T2> I2>
struct ZipGenerator {
  I1 begin1, end1;
  I2 begin2, end2;

  inline ZipGenerator(I1 begin1, I1 end1, I2 begin2, I2 end2)
    : begin1(begin1), end1(end1), begin2(begin2), end2(end2) {}
  
  inline Pair<T1, T2> operator()(size_t count) {
    if (begin1 == end1 || begin2 == end2) throw GeneratorException();
    return {*begin1++, *begin2++};
  }
};

} /* impl */

// yield from generator ??? maybe not yield
inline void yield() {
  throw GeneratorException();
}

inline Generator<size_t> range(size_t begin, size_t end = -1, size_t step = 1) {
  return {RangeGenerator(begin, end, step)};
}

template <typename T, Iterable<T> I>
inline Generator<Pair<size_t, T>> enumerate(const I& i) {
  return {Enumerator<T, decltype(i.cbegin())>(i.cbegin(), i.cend())};
}

template <typename T1, typename T2, Iterable<T1> I1, Iterable<T2> I2>
inline Generator<Pair<T1, T2>> zip(const I1& i1, const I2& i2) {
  return {ZipGenerator<T1, T2, decltype(i1.cbegin()), decltype(i2.cbegin())>(
    i1.cbegin(), i1.cend(), i2.cbegin(), i2.cend()
  )};
}

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_GENERATOR_H_ */