#include <cstdio>
#include <memory>

template <typename T>
class Iterator {
 public:
  virtual T& get() = 0;
  virtual void advance() = 0;
  virtual bool equals(const Iterator& rhs) = 0;

  T& operator*() {
    return get();
  }

  Iterator& operator++() {
    advance();
    return *this;
  }

  Iterator& operator++(int) {
    advance();
    return *this;
  }

  bool operator==(const Iterator& rhs) {
    return equals(rhs);
  }

  bool operator!=(const Iterator& rhs) {
    return !equals(rhs);
  }
};

template <typename T>
class Collection {};

template <typename T>
class Sequence : public Collection<T> {
 public:
  virtual std::unique_ptr<Iterator<T>> begin() = 0;
  virtual std::unique_ptr<Iterator<T>> end() = 0;
};

template <typename T>
class Array : public Sequence<T> {
 public:
  class Iterator : public ::Iterator<T> {
   public:
    Iterator() {}
    Iterator(Array<T>* array, size_t index) : m_array(array), m_index(index) {}

    T& get() override {
      return m_array->get(m_index);
    }

    void advance() override {
      m_index++;
    }

    bool equals(const ::Iterator<T>& rhs) override {
      if (const Iterator* it = dynamic_cast<const Iterator*>(&rhs))
        return m_index == it->m_index;
      return false;
    }

   private:
    Array<T>* m_array = nullptr;
    size_t m_index = 0;
  };

 public:
  Array() {
    m_buffer = new T[8]; // FIXME: magic number
    m_capacity = 8;
  }

  T& get(size_t index) const {
    return m_buffer[index];
  }

  size_t size() const {
    return m_size;
  }

  size_t capacity() const {
    return m_capacity;
  }

  size_t append(T& element) {
    if (m_size + 1 >= m_capacity) reserve(m_capacity+1, 2);
    m_buffer[m_size++] = element;
    return m_size-1;
  }

  size_t append(const T& element) {
    if (m_size + 1 >= m_capacity) reserve(m_capacity+1, 2);
    m_buffer[m_size++] = element;
    return m_size-1;
  }

  T& pop() {
    return m_buffer[--m_size];
  }

  void reserve(size_t capacity, size_t coefficient = 0) {
    if (m_capacity < capacity) {
      T* buffer = m_buffer;
      if (coefficient) {
        m_capacity *= coefficient;
      } else {
        m_capacity = capacity;
      }
      m_buffer = new T[m_capacity];
      for (size_t i = 0; i < m_size; i++) {
        m_buffer[i] = std::move(buffer[i]);
      }
      delete [] buffer;
    }
  }

  T& operator[](size_t index) {
    return get(index);
  }

  std::unique_ptr<::Iterator<T>> begin() override {
    return std::make_unique<Iterator>(this, 0);
  }
  std::unique_ptr<::Iterator<T>> end() override {
    return std::make_unique<Iterator>(this, m_size);
  }

 private:
  T* m_buffer = nullptr;
  size_t m_capacity = 0;
  size_t m_size = 0;
};


int main() {
  Array<int> ia;
  ia.append(10);
  ia.append(20);
  ia.append(30);

  for (size_t i = 0; i < ia.size(); i++) {
    printf("%zu: %d\n", i, ia[i]);
  }

  // for (auto it = ia.begin(); it != ia.end(); (*it)++) {
  //   printf("%d\n", *(*it));
  // }
}
