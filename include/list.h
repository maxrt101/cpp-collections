#ifndef _MRT_COLLECTIONS_LIST_H_
#define _MRT_COLLECTIONS_LIST_H_ 1

#include <initializer_list>
#include <functional>
#include <concepts>
#include <cstdlib>
#include <mrt/utils/constants.h>
#include <mrt/array.h>

namespace mrt {

template <typename T>
class List {
 public:
  struct Node {
    T value;
    Node* prev = nullptr;
    Node* next = nullptr;

    inline Node() {}
    inline Node(T value) : value(value) {}
    inline Node(T value, Node* prev) : value(value), prev(prev) {}
    inline Node(T value, Node* prev, Node* next) : value(value), prev(prev), next(next) {}

    inline virtual ~Node() {}
  };

  class Iterator {
   public:
    inline Iterator() {}
    inline Iterator(const Iterator& rhs) : m_list(rhs.m_list), m_node(rhs.m_node) {}
    inline Iterator(List* list) : m_list(list) {}
    inline Iterator(List* list, Node* node) : m_list(list), m_node(node) {}

    inline virtual ~Iterator() {}

    inline Node* node() const { return m_node; }
    inline List* list() const { return m_list; }

    Iterator& operator=(const Iterator& rhs) {
      m_list = rhs.m_list;
      m_node = rhs.m_node;
      return *this;
    }

    T& operator*() { return m_node->value; }

    const T& operator*() const { return m_node.value; }

    bool operator==(const Iterator& rhs) const { return m_list == rhs.m_list && m_node == rhs.m_node; }
    bool operator!=(const Iterator& rhs) const { return m_list == rhs.m_list && m_node != rhs.m_node; }

    Iterator& operator++() {
      if (m_node) m_node = m_node->next;
      return *this;
    }

    Iterator operator++(int) {
      Iterator it = *this;
      if (m_node) m_node = m_node->next;
      return it;
    }

    Iterator& operator--() {
      m_node = m_node ? m_node->prev : m_list->m_tail;
      return *this;
    }

    Iterator operator--(int) {
      Iterator it = *this;
      m_node = m_node ? m_node->prev : m_list->m_tail;
      return it;
    }

   private:
    List* m_list = nullptr;
    Node* m_node = nullptr;
  };

  class ConstIterator {
   public:
    inline ConstIterator() {}
    inline ConstIterator(const ConstIterator& rhs) : m_list(rhs.m_list), m_node(rhs.m_node) {}
    inline ConstIterator(const List* list) : m_list(list) {}
    inline ConstIterator(const List* list, Node* node) : m_list(list), m_node(node) {}

    inline virtual ~ConstIterator() {}

    inline Node* node() const { return m_node; }
    inline const List* list() const { return m_list; }

    ConstIterator& operator=(const ConstIterator& rhs) {
      m_list = rhs.m_list;
      m_node = rhs.m_node;
      return *this;
    }

    const T& operator*() const { return m_node->value; }

    bool operator==(const ConstIterator& rhs) const { return m_node == rhs.m_node; }
    bool operator!=(const ConstIterator& rhs) const { return m_node != rhs.m_node; }

    ConstIterator& operator++() {
      if (m_node) m_node = m_node->next;
      return *this;
    }

    ConstIterator operator++(int) {
      ConstIterator it = *this;
      if (m_node) m_node = m_node->next;
      return it;
    }

    ConstIterator& operator--() {
      m_node = m_node ? m_node->prev : m_list->m_tail;
      return *this;
    }

    ConstIterator operator--(int) {
      ConstIterator it = *this;
      m_node = m_node ? m_node->prev : m_list->m_tail;
      return it;
    }

   private:
    const List* m_list = nullptr;
    Node* m_node = nullptr;
  };

 public:
  inline List() {}

  inline List(const List& rhs) {
    for (Node* node = rhs.m_head; node; node = node->next) {
      append(node->value);
    }
  }

  inline List(std::initializer_list<T> il) {
    for (auto x : il) {
      append(x);
    }
  }

  inline virtual ~List() {
    clear();
  }

  inline size_t size() const { return m_size; }
  inline Iterator head() const { return Iterator(this, m_head); }
  inline Iterator tail() const { return Iterator(this, m_tail); }

  inline Iterator begin() { return Iterator(this, m_head); }
  inline Iterator end() { return Iterator(this, m_tail); }

  inline ConstIterator cbegin() const { return ConstIterator(this, m_head); }
  inline ConstIterator cend() const { return ConstIterator(this); }

  inline void append(const T& element) {
    if (!m_tail) {
      m_tail = new Node(element);
      m_head = m_tail;
      m_size++;
    } else {
      m_tail = insertAfter(m_tail, element);
    }
  }

  inline void prepend(const T& element) {
    if (!m_head) {
      m_head = new Node(element);
      m_tail = m_head;
      m_size++;
    } else {
      m_head = insertBefore(m_head, element);
    }
  }

  inline T pop() {
    if (m_head) {
      T value = std::move(m_tail->value);
      removeNode(m_tail);
      return value;
    }
    return T{};
  }

  inline void insert(size_t index, const T& element) {
    repairHeadTail(insertBefore(getNode(index), element));
  }

  inline void insert(const Iterator& it, const T& element) {
    repairHeadTail(insertBefore(getNode(it), element));
  }

  inline void remove(size_t index) {
    removeNode(getNode(index));
  }

  inline void remove(const Iterator& it) {
    removeNode(getNode(it));
  }  

  inline void clear() {
    for (Node* node = m_head; node; node = node->next) {
      delete node;
    }
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
  }

  inline bool contains(const T& value) const {
    for (Node* node = m_head; node; node = node->next) {
      if (node->value == value) {
        return true;
      }
    }

    return false;
  }

  inline List slice(size_t start, size_t end = 0) const {
    List result;

    if (end == 0) {
      size_t index = 0;
      for (Node* node = m_head; node; node = node->next) {
        if (index >= start) return result;
        result.append(node->value);
        index++;
      }
    } else {
      size_t index = 0;
      for (Node* node = m_head; node; node = node->next) {
        if (index < start) {
          index++;
          continue;
        }
        if (index >= end) return result;
        result.append(node->value);
        index++;
      }
    }

    return result;
  }

  inline Array<size_t> find(const T& value, size_t startIdx = 0) const {
    Array<size_t> indexes;

    size_t index = 0;
    for (Node* node = m_head; node; node = node->next) {
      if (index >= startIdx) {
        if (node->value == value) {
          indexes.append(index);
        }
      }
      index++;
    }

    return indexes;
  }

  inline size_t lfind(const T& value, size_t startIdx = 0) const {
    size_t index = 0;
    for (Node* node = m_head; node; node = node->next) {
      if (index >= startIdx) {
        if (node->value == value) {
          return index;
        }
      }
      index++;
    }

    return nidx;
  }

  inline size_t rfind(const T& value, size_t startIdx = nidx) const {
    size_t index = m_size-1;
    for (Node* node = m_tail; node; node = node->prev) {
      if (index <= startIdx) {
        if (node->value == value) {
          return index;
        }
      }
      index--;
    }

    return nidx;
  }

  inline List unique() {
    List result;

    for (auto& value : *this) {
      if (!result.contains(value)) {
        result.append(value);
      }
    }

    return result;
  }

  inline void foreach(std::function<void(const T&)> f) {
    for (Node* node = m_head; node; node = node->next) {
      f(node->value);
    }
  }

  inline List filter(std::function<bool(const T&)> pred) {
    List result;
    for (Node* node = m_head; node; node = node->next) {
      if (pred(node->value)) {
        result.append(node->value);
      }
    }
    
    return result;
  }

  template <typename R>
  inline R reduce(std::function<R(R, const T&)> reducer, R startValue = {}) {
    R result = startValue;
    for (Node* node = m_head; node; node = node->next) {
      result = reducer(result, node->value);
    }
    return result;
  }

  template <typename R>
  inline R reduceRight(std::function<R(R, const T&)> reducer, R startValue = {}) {
    R result = startValue;
    for (Node* node = m_tail; node; node = node->prev) {
      result = reducer(result, node->value);
    }
    return result;
  }

  template <typename R = T>
  inline List<R> map(std::function<R(const T&)> mapper) const {
    List<R> result;
    for (Node* node = m_head; node; node = node->next) {
      result.append(mapper(node->value));
    }
    return result;
  }

  inline T& get(size_t index, T defaultValue) {
    Node* node = getNode(index);
    return node ? node->value : defaultValue;
  }

  inline const T& get(size_t index, const T& defaultValue) const {
    Node* node = getNode(index);
    return node ? node->value : defaultValue;
  }

  inline T& operator[](size_t index) {
    return getNode(index)->value;
  }

  inline const T& operator[](size_t index) const {
    return getNode(index)->value;
  }

  inline List& operator=(const List& rhs) {
    clear();
    for (Node* node = rhs.m_head; node; node = node->next) {
      append(node->value);
    }
    return *this;
  }

  inline bool operator==(const List& rhs) const {
    if (m_size != rhs.m_size) return false;

    Node *node = m_head, *rnode = rhs.m_head;
    while (1) {
      if (!node && !rnode) return true;
      if (!node || !rnode) return false;

      if (node->value != rnode->value) return false;

      node = node->next;
      rnode = rnode->next;
    }

    return true;
  }

  inline bool operator!=(const List& rhs) const {
    if (m_size != rhs.m_size) return true;

    Node *node = m_head, *rnode = rhs.m_head;
    while (1) {
      if (!node && !rnode) return false;
      if (!node || !rnode) return true;

      if (node->value != rnode->value) return true;

      node = node->next;
      rnode = rnode->next;
    }

    return false;
  }

  inline List operator+(const List& rhs) const {
    List result;

    for (Node* node = m_head; node; node = node->next) {
      result.append(node->value);
    }

    for (Node* node = rhs.m_head; node; node = node->next) {
      result.append(node->value);
    }

    return result;
  }

  inline List operator&(const List& rhs) const {
    List result;

    for (Node* node = m_head; node; node = node->next) {
      if (rhs.contains(node->value)) {
        result.append(node->value);
      }
    }

    return result;
  }

  inline List operator|(const List& rhs) const {
    List result;

    for (Node* node = m_head; node; node = node->next) {
      if (!rhs.contains(node->value)) {
        result.append(node->value);
      }
    }

    return result;
  }

 private:
  inline Node* getNode(size_t index) {
    Node* node = nullptr;
    if (node >= 0) {
      node = m_head;
      for (int i = 0; i < index; i++) {
        if (!node) return nullptr;
        node = node->next;
      }
    } else {
      node = m_tail;
      for (int i = 0; i > index; i--) {
        if (!node) return nullptr;
        node = node->prev;
      }
    }
    return node;
  }

  inline Node* getNode(const Iterator& it) {
    return it.node();
  }

  inline Node* insertAfter(Node* node, const T& value) {
    if (!node) return nullptr;
    Node* newNode = new Node(value, node, node->next);
    if (node->next)
      node->next->prev = newNode;
    node->next = newNode;
    m_size++;
    return newNode;
  }

  inline Node* insertBefore(Node* node, const T& value) {
    if (!node) return nullptr;
    Node* newNode = new Node(value, node->prev, node);
    if (node->prev)
      node->prev->next = newNode;
    node->prev = newNode;
    m_size++;
    return newNode;
  }

  inline void removeNode(Node* node) {
    if (!node) return;

    if (node->prev) {
      node->prev->next = node->next;
      // if (node->next)
    } else {
      m_head = node->next;
    }

    if (node->next) {
      node->next->prev = node->prev;
    } else {
      m_tail = node->prev;
    }

    m_size--;

    delete node;
  }

  inline void repairHeadTail(Node* node) {
    if (!node->prev) m_head = node;
    if (!node->next) m_tail = node;
  }

 private:
  Node* m_head = nullptr;
  Node* m_tail = nullptr;
  size_t m_size = 0;
};

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_LIST_H_ */