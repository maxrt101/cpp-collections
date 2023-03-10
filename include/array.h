#ifndef _MRT_COLLECTIONS_ARRAY_H_
#define _MRT_COLLECTIONS_ARRAY_H_ 1

#include <initializer_list>
#include <functional>
#include <concepts>
#include <cstdlib>
#include <mrt/utils/constants.h>
#include <mrt/sort/merge.h>
#include <mrt/sort.h>

namespace mrt {

template <typename T>
class Array {
 public:
  class Iterator {
   public:
    inline Iterator() {}
    inline Iterator(const Iterator& rhs) : m_array(rhs.m_array), m_index(rhs.m_index) {}
    inline Iterator(Array* array, size_t index) : m_array(array), m_index(index) {}

    inline virtual ~Iterator() {}

    Array& array() const { return *m_array; }
    size_t index() const { return m_index; }

    Iterator& operator=(const Iterator& rhs) {
      m_array = rhs.m_array;
      m_index = rhs.m_index;
      return *this;
    }

    T& operator*() { return (*m_array)[m_index]; }

    const T& operator*() const { return (*m_array)[m_index]; }

    bool operator==(const Iterator& rhs) const { return m_index == rhs.m_index; }
    bool operator!=(const Iterator& rhs) const { return m_index != rhs.m_index; }

    Iterator& operator++() {
      m_index++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator it = *this;
      m_index++;
      return it;
    }

    Iterator& operator--() {
      m_index--;
      return *this;
    }

    Iterator operator--(int) {
      Iterator it = *this;
      m_index--;
      return it;
    }

    Iterator operator+(int rhs) {
      Iterator it = *this;
      it.m_index += rhs;
      return it;
    }

    Iterator operator+(const Iterator& rhs) {
      Iterator it = *this;
      it.m_index += rhs.m_index;
      return it;
    }

    Iterator operator-(int rhs) {
      Iterator it = *this;
      it.m_index -= rhs;
      return it;
    }

    Iterator operator-(const Iterator& rhs) {
      Iterator it = *this;
      it.m_index -= rhs.m_index;
      return it;
    }

    Iterator& operator+=(int rhs) {
      m_index += rhs;
      return *this;
    }

    Iterator& operator+=(const Iterator& rhs) {
      m_index += rhs.m_index;
      return *this;
    }

    Iterator& operator-=(int rhs) {
      m_index -= rhs;
      return *this;
    }

    Iterator& operator-=(const Iterator& rhs) {
      m_index -= rhs.m_index;
      return *this;
    }

   private:
    Array* m_array = nullptr;
    size_t m_index = 0;
  };

  class ConstIterator {
   public:
    inline ConstIterator() {}
    inline ConstIterator(const ConstIterator& rhs) : m_array(rhs.m_array), m_index(rhs.m_index) {}
    inline ConstIterator(const Array* array, size_t index) : m_array(array), m_index(index) {}

    inline virtual ~ConstIterator() {}

    Array& array() const { return *m_array; }
    size_t index() const { return m_index; }

    ConstIterator& operator=(const ConstIterator& rhs) {
      m_array = rhs.m_array;
      m_index = rhs.m_index;
      return *this;
    }

    const T& operator*() const { return (*m_array)[m_index]; }

    bool operator==(const ConstIterator& rhs) const { return m_index == rhs.m_index; }
    bool operator!=(const ConstIterator& rhs) const { return m_index != rhs.m_index; }

    ConstIterator& operator++() {
      m_index++;
      return *this;
    }

    ConstIterator operator++(int) {
      ConstIterator it = *this;
      m_index++;
      return it;
    }

    Iterator& operator--() {
      m_index--;
      return *this;
    }

    ConstIterator operator--(int) {
      ConstIterator it = *this;
      m_index--;
      return it;
    }

    Iterator operator+(int rhs) {
      Iterator it = *this;
      it.m_index += rhs;
      return it;
    }

    Iterator operator+(const Iterator& rhs) {
      Iterator it = *this;
      it.m_index += rhs.m_index;
      return it;
    }

    Iterator operator-(int rhs) {
      Iterator it = *this;
      it.m_index -= rhs;
      return it;
    }

    Iterator operator-(const Iterator& rhs) {
      Iterator it = *this;
      it.m_index -= rhs.m_index;
      return it;
    }

    Iterator& operator+=(int rhs) {
      m_index += rhs;
      return *this;
    }

    Iterator& operator+=(const Iterator& rhs) {
      m_index += rhs.m_index;
      return *this;
    }

    Iterator& operator-=(int rhs) {
      m_index -= rhs;
      return *this;
    }

    Iterator& operator-=(const Iterator& rhs) {
      m_index -= rhs.m_index;
      return *this;
    }

   private:
    const Array* m_array = nullptr;
    size_t m_index = 0;
  };

  constexpr static size_t INITIAL_SIZE = 8;
  constexpr static size_t GROWTH_FACTOR = 2;

 public:
  inline Array() {
    reserve(INITIAL_SIZE);
  }

  inline Array(const Array& rhs) {
    operator=(rhs);
  }

  inline Array(std::initializer_list<T> il) {
    reserve(il.size());
    for (auto x : il) {
      append(x);
    }
  }

  inline virtual ~Array() {
    clear();
  }

  static inline Array empty(size_t size) {
    Array result;
    result.reserve(size);
    return result;
  }

  static inline Array filled(size_t size, T element) {
    Array result;
    result.reserve(size);
    for (size_t i = 0; i < size; i++) {
      result.append(element);
    }
    return result;
  }

  inline size_t size() const { return m_size; }
  inline size_t capacity() const { return m_capacity; }
  inline T* data() const { return m_buffer; }

  inline Iterator begin() { return Iterator(this, 0); }
  inline Iterator end() { return Iterator(this, m_size); }

  inline ConstIterator cbegin() const { return ConstIterator(this, 0); }
  inline ConstIterator cend() const { return ConstIterator(this, m_size); }

  inline void append(const T& element) {
    if (m_size + 1 >= m_capacity) {
      reserve(m_capacity * GROWTH_FACTOR);
    }
    m_buffer[m_size++] = element;
  }
  
  inline void prepend(const T& element) {
    if (m_size + 1 >= m_capacity) {
      reserve(m_capacity * GROWTH_FACTOR);
    }
    for (int i = m_size; i > 0; i--) {
      m_buffer[i] = m_buffer[i-1];
    }
    m_buffer[0] = element;
    m_size++;
  }

  inline T pop() {
    m_size--;
    return std::move(m_buffer[m_size]);
  }

  inline void insert(size_t index, const T& element) {
    if (m_size + 1 >= m_capacity) {
      reserve(m_capacity * GROWTH_FACTOR);
    }
    for (size_t i = m_size; i > index; i--) {
      m_buffer[i] = m_buffer[i-1];
    }
    m_buffer[index] = element;
    m_size++;
  }

  inline void insert(const Iterator& it, const T& element) {
    insert(it.index(), element);
  }

  inline void remove(size_t index) {
    for (size_t i = index; i < m_size; i++) {
      m_buffer[i] = m_buffer[i+1];
    }
    m_size--;
  }

  inline void remove(size_t start, size_t end) {
    for (size_t i = start; i < m_size; i++) {
      m_buffer[i] = m_buffer[i + end - start];
    }
    m_size -= end - start;
  }

  inline void remove(const Iterator& it) {
    remove(it.index());
  }

  inline void remove(const Iterator& start, const Iterator& end) {
    remove(start.index(), end.index());
  }

  inline void reserve(size_t size) {
    if (m_buffer) {
      if (size > m_capacity) {
        T* buffer = m_buffer;
        m_buffer = new T[size];
        for (size_t i = 0; i < m_size; i++) {
          m_buffer[i] = buffer[i];
        }
        delete [] buffer;
        m_capacity = size;
      }
    } else {
      m_capacity = size;
      m_size = 0;
      m_buffer = new T[size];
    }
  }

  inline void clear() {
    if (m_buffer) {
      delete [] m_buffer;
      m_size = 0;
      m_capacity = 0;
      m_buffer = nullptr;
    }
  }

  inline bool contains(const T& value) const {
    for (size_t i = 0; i < m_size; i++) {
      if (m_buffer[i] == value) {
        return true;
      }
    }

    return false;
  }

  inline Array slice(size_t start, size_t end = 0) const {
    Array result;

    if (end == 0) {
      for (size_t i = 0; i < start; i++) {
        result.append(m_buffer[i]);
      }
    } else {
      for (size_t i = start; i < end; i++) {
        result.append(m_buffer[i]);
      }
    }

    return result;
  }

  inline Array<size_t> find(const T& value, size_t startIdx = 0) const {
    Array<size_t> indexes;

    for (size_t i = startIdx; i < m_size; i++) {
      if (m_buffer[i] == value) {
        indexes.append(i);
      }
    }

    return indexes;
  }

  inline size_t lfind(const T& value, size_t startIdx = 0) const {
    for (size_t i = startIdx; i < m_size; i++) {
      if (m_buffer[i] == value) {
        return i;
      }
    }

    return nidx;
  }

  inline size_t rfind(const T& value, size_t startIdx = 0) const {
    for (size_t i = (startIdx ? startIdx : m_size-1); i >= 0; i--) {
      if (m_buffer[i] == value) {
        return i;
      }
    }

    return nidx;
  }

  inline Array unique() {
    Array result;

    for (size_t i = 0; i < m_size; i++) {
      if (!result.contains(m_buffer[i])) {
        result.append(m_buffer[i]);
      }
    }

    return result;
  }

  inline void reverse() {
    *this = reversed();
  }

  inline Array reversed() {
    Array result;

    for (size_t i = m_size; i > 0; i--) {
      result.append(m_buffer[i-1]);
    }

    return result;
  }

  template <Sorter<T, Array> S = MergeSort>
  inline void sort(SortComparator<T> comparator = asc<T>, S sorter = {}) {
    sorter.sort(comparator, *this);
  }

  template <Sorter<T, Array> S = MergeSort>
  inline Array sorted(SortComparator<T> comparator = asc<T>, S sorter = {}) {
    Array result = *this;

    sorter.sort(comparator, result);

    return result;
  }

  inline void foreach(std::function<void(const T&)> f) const {
    for (size_t i = 0; i < m_size; i++) {
      f(m_buffer[i]);
    }
  }

  inline Array filter(std::function<bool(const T&)> pred) const {
    Array result;
    result.reserve(size());
    for (size_t i = 0; i < m_size; i++) {
      if (pred(m_buffer[i])) {
        result.append(m_buffer[i]);
      }
    }
    return result;
  }

  template <typename R = T>
  inline R reduce(std::function<R(R, const T&)> reducer, R startValue = {}) const {
    R result = startValue;
    for (size_t i = 0; i < m_size; i++) {
      result = reducer(result, m_buffer[i]);
    }
    return result;
  }

  template <typename R = T>
  inline R reduceRight(std::function<R(R, const T&)> reducer, R startValue = {}) const {
    R result = startValue;
    for (size_t i = m_size; i > 0; i--) {
      result = reducer(result, m_buffer[i-1]);
    }
    return result;
  }

  template <typename R = T>
  inline Array<R> map(std::function<R(const T&)> mapper) const {
    Array<R> result;
    for (size_t i = 0; i < m_size; i++) {
      result.append(mapper(m_buffer[i]));
    }
    return result;
  }

  inline T& get(size_t index, T defaultValue) {
    if (index < 0) return (m_size - index >= 0) ? m_buffer[m_size - index] : defaultValue;
    return (index < m_size) ? m_buffer[index] : defaultValue;
  }

  inline const T& get(size_t index, const T& defaultValue) const {
    if (index < 0) return (m_size - index >= 0) ?  m_buffer[m_size - index] : defaultValue;
    return (index < m_size) ? m_buffer[index] : defaultValue;
  }

  inline T& operator[](size_t index) {
    if (index < 0) return m_buffer[m_size - index];
    return m_buffer[index];
  }

  inline const T& operator[](size_t index) const {
    if (index < 0) return m_buffer[m_size - index];
    return m_buffer[index];
  }

  inline Array& operator=(const Array& rhs) {
    clear();
    reserve(rhs.m_capacity);
    for (size_t i = 0; i < rhs.m_size; i++) {
      append(rhs.m_buffer[i]);
    }
    return *this;
  }

  inline bool operator==(const Array& rhs) const {
    if (m_size != rhs.m_size) return false;
    if (m_buffer == rhs.m_buffer) return true;

    for (size_t i = 0; i < m_size; i++) {
      if (m_buffer[i] != rhs.m_buffer[i]) {
        return false;
      }
    }

    return true;
  }

  inline bool operator!=(const Array& rhs) const {
    if (m_size != rhs.m_size) return true;
    if (m_buffer == rhs.m_buffer) return false;

    for (size_t i = 0; i < m_size; i++) {
      if (m_buffer[i] != rhs.m_buffer[i]) {
        return true;
      }
    }

    return false;
  }

  inline Array operator+(const Array& rhs) const {
    Array result = *this;

    for (size_t i = 0; i < rhs.m_size; i++) {
      result.append(rhs[i]);
    }

    return result;
  }

  inline Array& operator+=(const Array& rhs) {
    for (size_t i = 0; i < rhs.m_size; i++) {
      append(rhs[i]);
    }
    return *this;
  }

  inline Array& operator+=(const T& rhs) {
    append(rhs);
    return *this;
  }

  inline Array operator&(const Array& rhs) const {
    Array result;

    for (size_t i = 0; i < m_size; i++) {
      if (rhs.contains(m_buffer[i])) {
        result.append(m_buffer[i]);
      }
    }

    return result;
  }

  inline Array operator|(const Array& rhs) const {
    Array result;

    for (size_t i = 0; i < m_size; i++) {
      if (!rhs.contains(m_buffer[i])) {
        result.append(m_buffer[i]);
      }
    }

    return result;
  }

 private:
  size_t m_size = 0;
  size_t m_capacity = 0;
  T* m_buffer = nullptr;
};

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_ARRAY_H_ */