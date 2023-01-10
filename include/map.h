#ifndef _MRT_COLLECTIONS_MAP_H_
#define _MRT_COLLECTIONS_MAP_H_ 1

#include <initializer_list>
#include <functional>
#include <exception>
#include <cstdlib>
#include <mrt/utils/concepts.h>
#include <mrt/array.h>
#include <mrt/pair.h>

namespace mrt {

template <typename T>
size_t getHash(const T& value) {
  return std::hash(value);
}

template <Hashable T>
size_t getHash(const T& value) {
  return value.hash();
}

template <typename T>
size_t getHash(const int& value) {
  return value;
}

template <typename T>
size_t getHash(const float& value) {
  return value;
}

template <typename T>
size_t getHash(const void* value) {
  return (size_t) value;
}

template <typename K, typename V>
class Map {
 public:
  struct NoSuchElementException : public std::exception {
    inline NoSuchElementException() {}
  };

  struct Node {
    Pair<K, V> data;
    Node* next = nullptr;

    inline Node() {}
    inline Node(K k) : data(k) {}
    inline Node(K k, V v) : data(k, v) {}
    inline Node(K k, V v, Node* next) : data(k, v), next(next) {}

    inline virtual ~Node() {}
    
    K& key() {
      return data._1;
    }

    V& value() {
      return data._2;
    }

    Pair<K, V>& pair() {
      return data;
    }

    inline void addToChain(Node* node) {
      Node* tmp = next;
      while (tmp->next) tmp = tmp->next;
      tmp->next = node;
    }
  };

  class Iterator {
   public:
    inline Iterator() {}
    inline Iterator(const Iterator& rhs) : m_map(rhs.m_map), m_index(rhs.m_index), m_node(rhs.m_node) {}
    inline Iterator(Map* map) : m_map(map) {}
    inline Iterator(Map* map, size_t index) : m_map(map), m_index(index) {}
    inline Iterator(Map* map, size_t index, Node* node) : m_map(map), m_index(index), m_node(node) {}

    inline virtual ~Iterator() {}

    inline Map* map() const { return m_map; }
    inline size_t index() const { return m_index; }
    inline Node* node() const { return m_node; }

    Iterator& operator=(const Iterator& rhs) {
      m_map = rhs.m_map;
      m_node = rhs.m_node;
      m_index = rhs.m_index;
      return *this;
    }

    Pair<K, V>& operator*() {
      if (m_node) return m_node->pair();
      if (m_map->existsAtIndex(m_index)) return m_map->getAtIndex(m_index)->pair();
      (*this)++;
      return *(*this);
    }

    bool operator==(const Iterator& rhs) const { return m_map == rhs.m_map && m_node == rhs.m_node && m_index == rhs.m_index; }
    bool operator!=(const Iterator& rhs) const { return m_map == rhs.m_map && m_node != rhs.m_node && m_index != rhs.m_index; }

    Iterator& operator++() {
      if (m_node) {
        m_node = m_node->next;
      } else {
        while (!m_map->existsAtIndex(m_index)) {
          m_index += 1;
        }
        m_node = m_map->getAtIndex(m_index);
      }
      return *this;
    }

    Iterator operator++(int) {
      Iterator it = *this;
      if (m_node) {
        m_node = m_node->next;
      } else {
        while (!m_map->existsAtIndex(m_index)) {
          m_index += 1;
        }
        m_node = m_map->getAtIndex(m_index);
      }
      return it;
    }

    Iterator& operator--() {
     if (m_node) {
        m_node = m_node->next;
      } else {
        if (m_index < m_map->capacity()) {
          while (!m_map->existsAtIndex(m_index)) {
            m_index -= 1;
          }
          m_node = m_map->getAtIndex(m_index);
        }
      }
      return *this;
    }

    Iterator operator--(int) {
      Iterator it = *this;
      if (m_node) {
        m_node = m_node->next;
      } else {
        if (m_index < m_map->capacity()) {
          while (!m_map->existsAtIndex(m_index)) {
            m_index -= 1;
          }
          m_node = m_map->getAtIndex(m_index);
        }
      }
      return it;
    }

   private:
    Map* m_map = nullptr;
    Node* m_node = nullptr;
    size_t m_index = 0;
  };

  class ConstIterator {
   public:
    inline ConstIterator() {}
    inline ConstIterator(const ConstIterator& rhs) : m_map(rhs.m_map), m_index(rhs.m_index), m_node(rhs.m_node) {}
    inline ConstIterator(Map* map) : m_map(map) {}
    inline ConstIterator(Map* map, size_t index) : m_map(map), m_index(index) {}
    inline ConstIterator(Map* map, size_t index, Node* node) : m_map(map), m_index(index), m_node(node) {}

    inline virtual ~ConstIterator() {}

    inline Map* map() const { return m_map; }
    inline size_t index() const { return m_index; }
    inline const Node* node() const { return m_node; }

    ConstIterator& operator=(const ConstIterator& rhs) {
      m_map = rhs.m_map;
      m_node = rhs.m_node;
      m_index = rhs.m_index;
      return *this;
    }

    const Pair<K, V>& operator*() {
      if (m_node) return m_node->pair();
      if (m_map->existsAtIndex(m_index)) return m_map->getAtIndex(m_index)->pair();
      (*this)++;
      return *(*this);
    }

    bool operator==(const ConstIterator& rhs) const { return m_map == rhs.m_map && m_node == rhs.m_node && m_index == rhs.m_index; }
    bool operator!=(const ConstIterator& rhs) const { return m_map == rhs.m_map && m_node != rhs.m_node && m_index != rhs.m_index; }

    ConstIterator& operator++() {
      if (m_node) {
        m_node = m_node->next;
      } else {
        while (!m_map->existsAtIndex(m_index)) {
          m_index += 1;
        }
        m_node = m_map->getAtIndex(m_index);
      }
      return *this;
    }

    ConstIterator operator++(int) {
      Iterator it = *this;
      if (m_node) {
        m_node = m_node->next;
      } else {
        while (!m_map->existsAtIndex(m_index)) {
          m_index += 1;
        }
        m_node = m_map->getAtIndex(m_index);
      }
      return it;
    }

    ConstIterator& operator--() {
     if (m_node) {
        m_node = m_node->next;
      } else {
        if (m_index < m_map->capacity()) {
          while (!m_map->existsAtIndex(m_index)) {
            m_index -= 1;
          }
          m_node = m_map->getAtIndex(m_index);
        }
      }
      return *this;
    }

    ConstIterator operator--(int) {
      Iterator it = *this;
      if (m_node) {
        m_node = m_node->next;
      } else {
        if (m_index < m_map->capacity()) {
          while (!m_map->existsAtIndex(m_index)) {
            m_index -= 1;
          }
          m_node = m_map->getAtIndex(m_index);
        }
      }
      return it;
    }

   private:
    Map* m_map = nullptr;
    Node* m_node = nullptr;
    size_t m_index = 0;
  };

  constexpr static size_t INITIAL_SIZE = 32;
  constexpr static double GROWTH_FACTOR = 2;
  constexpr static double MAX_LOAD_FACTOR = 0.75;

 public:
  inline Map() {
    recreateBuckets();
  }

  inline Map(const Map& rhs) {
    operator=(rhs);
  }

  inline Map(std::initializer_list<Pair<K, V>> il) {
    for (auto& [k, v] : il) {
      set(k, v);
    }
  }

  inline virtual ~Map() {
    clear();
  }

  inline size_t size() const {
    return m_size;
  }

  inline size_t capacity() const {
    return m_capacity;
  }

  inline double loadFactor() const {
    if (!m_capacity) return 0.0;
    return m_size / m_capacity;
  }

  inline void clear() {
    for (auto node : m_buckets) {
      if (node) delete node;
    }
    m_buckets.clear();
    m_capacity = 0;
    m_size = 0;
  }

  inline Iterator begin() { return Iterator(this, 0); }
  inline Iterator end() { return Iterator(this, m_capacity); }

  inline ConstIterator cbegin() const { return ConstIterator(this, 0); }
  inline ConstIterator cend() const { return ConstIterator(this, m_capacity); }

  inline void set(const K& key, const V& value) {
    addNewNode(key, value);
  }

  inline V& get(const K& key) {
    if (!m_capacity) recreateBuckets();
    size_t index = getHash(key) % m_capacity;

    if (!m_buckets[index]) {
      m_buckets[index] = new Node(key);
      return m_buckets[index]->value();
    }

    Node* node = m_buckets[index];
    while (node->key() != key && node->next) {
      node = node->next;
    }

    return node->value();
  }

  inline const V& get(const K& key) const {
    if (!m_capacity) throw NoSuchElementException();
    size_t index = getHash(key) % m_capacity;

    if (!m_buckets[index]) {
      return m_buckets[index]->value();
    }

    Node* node = m_buckets[index];
    while (node->key() != key && node->next) {
      node = node->next;
    }

    return node->value();
  }

  inline const V& get(const K& key, const V& defaultValue) const {
    return contains(key) ? get(key) : defaultValue;
  }

  inline void remove(const K& key) {
    if (!m_capacity) return;
    size_t index = getHash(key) % m_capacity;

    if (m_buckets[index]) {
      if (m_buckets[index]->key() == key) {
        Node* node = m_buckets[index];
        m_buckets[index] = m_buckets[index]->next;
        m_size -= 1;
        delete node;
        return;
      } else {
        Node* node = m_buckets[index];
        while (node->next) {
          if (node->next->key() == key) {
            node->next = node->next->next;
            m_size -= 1;
            delete node;
            return;
          }
          node = node->next;
        }
      }
    }

    throw NoSuchElementException();
  }

  inline Array<K> keys() const {
    Array<K> result;
    for (size_t i = 0; i < m_capacity; i++) {
      if (m_buckets[i]) {
        Node* node = m_buckets[i];
        while (node) {
          result.append(node->key());
          node = node->next;
        }
      }
    }
    return result;
  }

  inline Array<V> values() const {
    Array<V> result;
    for (size_t i = 0; i < m_capacity; i++) {
      if (m_buckets[i]) {
        Node* node = m_buckets[i];
        while (node) {
          result.append(node->value());
          node = node->next;
        }
      }
    }
    return result;
  }

  inline Array<Pair<K, V>> items() const {
    Array<Pair<K, V>> result;
    for (size_t i = 0; i < m_capacity; i++) {
      if (m_buckets[i]) {
        Node* node = m_buckets[i];
        while (node) {
          result.append(node->pair());
          node = node->next;
        }
      }
    }
    return result;
  }

  inline bool contains(const K& key) const {
    size_t index = getHash(key) % m_capacity;

    return m_buckets[index] != nullptr;
  }

  inline void foreach(std::function<void(const Pair<K, V>&)> f) {
    for (size_t i = 0; i < m_capacity; i++) {
      if (m_buckets[i]) {
        Node* node = m_buckets[i];
        while (node) {
          f(node->pair());
          node = node->next;
        }
      }
    }
  }

  inline Map filter(std::function<bool(const Pair<K, V>&)> pred) {
    Map result;
    for (size_t i = 0; i < m_capacity; i++) {
      if (m_buckets[i]) {
        Node* node = m_buckets[i];
        while (node) {
          if (pred(node->pair())) {
            result.set(node->key(), node->value());
          }
          node = node->next;
        }
      }
    }
    return result;
  }

  template <typename R>
  inline R reduce(std::function<R(R, const Pair<K, V>&)> reducer, R startValue = {}) {
    R result = startValue;
    for (size_t i = 0; i < m_capacity; i++) {
      if (m_buckets[i]) {
        Node* node = m_buckets[i];
        while (node) {
          result = reducer(result, node->pair());
          node = node->next;
        }
      }
    }
    return result;
  }

  template <typename NK = K, typename NV = V>
  inline Map<NK, NV> map(std::function<Pair<NK, NV>(const Pair<K, V>&)> mapper) const {
    Map<NK, NV> result;
    for (size_t i = 0; i < m_capacity; i++) {
      if (m_buckets[i]) {
        Node* node = m_buckets[i];
        while (node) {
          auto p = mapper(node->pair());
          result.set(p._1, p._2);
          node = node->next;
        }
      }
    }
    return result;
  }

  template <typename T>
  inline Array<T> flatMap(std::function<T(const Pair<K, V>&)> mapper) const {
    Array<T> result;
    for (size_t i = 0; i < m_capacity; i++) {
      if (m_buckets[i]) {
        Node* node = m_buckets[i];
        while (node) {
          result.append(mapper(node->pair()));
          node = node->next;
        }
      }
    }
    return result;
  }

  inline V& operator[](const K& key) {
    return get(key);
  }

  inline const V& operator[](const K& key) const {
    return get(key);
  }

  inline Map& operator=(const Map& rhs) {
    clear();
    m_capacity = rhs.m_capacity;
    recreateBuckets();
    for (size_t i = 0; i < rhs.m_capacity; i++) {
      if (rhs.m_buckets[i]) {
        Node* node = rhs.m_buckets[i];
        while (node) {
          set(node->key(), node->value());
          node = node->next;
        }
      }
    }
    return *this;
  }

  inline bool operator==(const Map& rhs) const {
    for (auto& [k, v] : items()) {
      if (!rhs.contains(k)) return false;
      if (rhs[k] != v) return false;
    }
    return true;
  }

  inline bool operator!=(const Map& rhs) const {
    for (auto& [k, v] : items()) {
      if (!rhs.contains(k)) return true;
      if (rhs[k] != v) return true;
    }
    return false;
  }

  inline Map operator+(const Map& rhs) const {
    Map result = *this;
    for (auto& k : rhs.keys()) {
      result[k] = rhs[k];
    }
    return result;
  }

 private:
  void recreateBuckets() {
    if (m_buckets.size()) {
      auto buckets = m_buckets;
      m_buckets = Array<Node*>::filled(m_capacity, nullptr);
      for (auto& node : m_buckets) {
        if (node) {
          addNewNode(node);
          Node* tmp = node;
          while (tmp) {
            addNewNode(tmp);
            tmp = tmp->next;
          }
        }
      }
      buckets.clear();
    } else {
      if (!m_capacity) m_capacity = INITIAL_SIZE;
      m_buckets = Array<Node*>::filled(m_capacity, nullptr);
    }
  }

  void addNewNode(Node* node) {
    if (!m_buckets.size()) recreateBuckets();
    if (loadFactor() >= MAX_LOAD_FACTOR) {
      m_capacity *= GROWTH_FACTOR;
      recreateBuckets();
    }

    if (!node) return;

    size_t index = getHash(node->key()) % m_capacity;

    if (m_buckets[index]) {
      if (m_buckets[index]->key() == node->key()) {
        m_buckets[index]->value() = node->value();
        delete node;
      } else {
        m_buckets[index]->addToChain(node);
      }
    } else {
      m_buckets[index] = node;
    }

    m_size += 1;
  }

  void addNewNode(const K& key, const V& value) {
    addNewNode(new Node(key, value));
  }

  Node* getAtIndex(size_t index) {
    return m_buckets[index];
  }

  bool existsAtIndex(size_t index) {
    return m_buckets[index] != nullptr;
  }

 private:
  size_t m_size = 0;
  size_t m_capacity = 0;
  Array<Node*> m_buckets;
};

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_MAP_H_ */