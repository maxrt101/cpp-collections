#ifndef _MRT_COLLECTIONS_STRING_H_
#define _MRT_COLLECTIONS_STRING_H_ 1

#include <string>
#include <cctype>
#include <mrt/array.h>
#include <mrt/list.h>
#include <mrt/pair.h>

namespace mrt {

template <typename T>
class BaseString;


template <typename T, typename CharT = char>
inline BaseString<CharT> toString(T value);

template <typename CharT = char>
inline BaseString<CharT> toString(BaseString<CharT>& value);

template <typename CharT = char>
inline BaseString<CharT> toString(int value);

template <typename CharT = char>
inline BaseString<CharT> toString(long value);

template <typename CharT = char>
inline BaseString<CharT> toString(long long value);

template <typename CharT = char>
inline BaseString<CharT> toString(unsigned value);

template <typename CharT = char>
inline BaseString<CharT> toString(unsigned long value);

template <typename CharT = char>
inline BaseString<CharT> toString(unsigned long value);

template <typename CharT = char>
inline BaseString<CharT> toString(float value);

template <typename CharT = char>
inline BaseString<CharT> toString(double value);

template <typename CharT = char>
inline BaseString<CharT> toString(void* value);

template <typename T1, typename T2, typename CharT = char>
inline BaseString<CharT> toString(const Pair<T1, T2>& value);

template <typename T, Collection<T> C, typename CharT = char>
inline BaseString<CharT> collectionToBaseString(const C& value);

template <typename T, typename CharT = char>
inline BaseString<CharT> toString(const Array<T>& value);

template <typename T, typename CharT = char>
inline BaseString<CharT> toString(const List<T>& value);


template <typename T = char>
class BaseString : public Array<T> {
 public:
  inline BaseString() : Array<T>() {}

  inline BaseString(const BaseString& s) {
    this->Array<T>::operator=(s);
  }

  inline BaseString(const T* s) : Array<T>() {
    append(s);
  }
  
  inline BaseString(const T* start, const T* end) : Array<T>() {
    if (!start || !end || start > end) return;
    while (start < end) {
      append(*start++);
    }
  }

  inline BaseString(const T* s, size_t length) : Array<T>() {
    for (size_t i = 0; i < length; i++) {
      append(s[i]);
    }
  }

  inline BaseString(const std::string& s) : Array<T>() {
    append(s);
  }

  inline BaseString(const Array<T>& a) : Array<T>() {
    this->Array<T>::operator=(a);
  }

  inline const char* c_str() const {
    return this->data();
  }

  inline std::string cpp_str() const {
    return std::string(this->data());
  }

  inline void append(const BaseString& s) {
    this->reserve(this->size() + s.size());
    for (size_t i = 0; i < s.size(); i++) {
      this->Array<T>::append(s[i]);
    }
  }

  inline void append(const T* s) {
    while (*s) {
      this->Array<T>::append(*s++);
    }
    this->Array<T>::reserve(this->size()+1);
    this->Array<T>::operator[](this->size()) = 0;
  }

  inline void append(T c) {
    this->Array<T>::append(c);
    this->Array<T>::reserve(this->size()+1);
    this->Array<T>::operator[](this->size()) = 0;
  }

  inline void append(const std::string& s) {
    this->reserve(this->size() + s.size() + 1);
    for (size_t i = 0; i < s.size(); i++) {
      this->Array<T>::append(s[i]);
    }
    this->Array<T>::operator[](this->size()) = 0;
  }

  inline const char* operator*() const {
    return this->data();
  }

  inline BaseString operator+(const BaseString& rhs) {
    BaseString result = *this;
    result.append(rhs);
    return result;
  }

  inline BaseString operator+(const std::string& rhs) {
    BaseString result = *this;
    result.append(rhs);
    return result;
  }

  inline BaseString operator+(const T* rhs) {
    BaseString result = *this;
    result.append(rhs);
    return result;
  }

  inline BaseString operator+(T rhs) {
    BaseString result = *this;
    result.append(rhs);
    return result;
  }

  inline BaseString& operator+=(const BaseString& rhs) {
    append(rhs);
    return *this;
  }

  inline BaseString& operator+=(const std::string& rhs) {
    append(rhs);
    return *this;
  }

  inline BaseString& operator+=(const T* rhs) {
    append(rhs);
    return *this;
  }

  inline BaseString& operator+=(T rhs) {
    this->Array<T>::append(rhs);
    return *this;
  }

  inline bool operator==(const BaseString& rhs) const {
    if (this->size() != rhs.size()) return false;
    return this->Array<T>::operator==(rhs);
  }

  inline bool operator==(const char* rhs) const {
    size_t i = 0;
    for (i = 0; i < this->size(); i++) {
      if (!rhs[i] || rhs[i] != this->operator[](i)) return false;
    }
    return !rhs[i];
  }

  inline bool operator!=(const BaseString& rhs) const {
    if (this->size() != rhs.size()) return true;
    return this->Array<T>::operator!=(rhs);
  }

  inline bool operator!=(const char* rhs) const {
    size_t i = 0;
    for (i = 0; i < this->size(); i++) {
      if (!rhs[i] || rhs[i] != this->operator[](i)) return true;
    }
    return rhs[i];
  }

  inline void insert(size_t index, const BaseString& s) {
    for (size_t i = 0; i < s.size(); i++) {
      this->Array<T>::insert(index + i, s[i]);
    }
  }

  // djb2 (k=33) http://www.cse.yorku.ca/~oz/hash.html
  inline size_t hash() const {
    size_t result = 5381;
    for (size_t i = 0; i < this->size(); i++) {
      result = ((result << 5) + result) + this->operator[](i); // hash * 33 + c
    }
    return result;
  }

  inline size_t find(T c, size_t startIdx = 0) const {
    return this->Array<T>::lfind(c, startIdx);
  }

  inline size_t find(const BaseString& pattern, size_t startIdx = 0) const {
    size_t patternHash = pattern.hash();
    for (size_t i = startIdx; i < this->size(); i++) {
      auto sub = this->substr(i, i + pattern.size());
      if (sub.hash() == patternHash && sub == pattern) {
        return i;
      }
    }
    return nidx;
  }

  template <typename... Args>
  inline BaseString format(Args&&... args) const {
    return BaseString::format(c_str(), args...);
  }

  inline BaseString substr(size_t start, size_t end = 0) const {
    return this->Array<T>::slice(start, end);
  }

  inline BaseString upper() const {
    return this->template map<T>([](auto ch) { return toupper(ch); });
  }

  inline BaseString lower() const {
    return this->template map<T>([](auto ch) { return tolower(ch); });
  }

  inline BaseString replace(T c, T r) const {
    BaseString result = *this;
    for (size_t i = 0; i < result.size(); i++) {
      if (result[i] == c) {
        result[i] = r;
      }
    }
    return result;
  }

  inline BaseString replace(const BaseString& s, const BaseString& r) const {
    BaseString result = *this;
    size_t index = 0, findIndex = 0;
    while (index < result.size()) {
      if ((findIndex = result.find(s, index)) != nidx) {
        result.remove(findIndex, findIndex + s.size());
        result.insert(findIndex, r);
        index = findIndex + r.size() - 1;
      }
      index++;
    }
    return result;
  }

  inline Array<BaseString> split(const BaseString& delimiter = " ") const {
    Array<BaseString> result;
    size_t last = 0, next = 0;
    while ((next = this->find(delimiter, last)) != nidx) {
      result.append(this->substr(last, next));
      last = next + 1;
    }
    result.append(this->substr(last, this->size()));
    return result;
  }

  inline bool startsWith(const BaseString& s) const {
    return find(s, 0) == 0;
  }

  inline bool endsWith(const BaseString& s) {
    size_t index = this->size() - s.size();
    return find(s, index) == index;
  }

  template <typename... Args>
  static inline BaseString format(const char* fmt, Args&&... args) {
    BaseString result;
    impl::format(fmt, result, args...);
    return result;
  }

  template <typename... Args>
  static inline BaseString format(const BaseString& fmt, Args&&... args) {
    BaseString result;
    impl::format(fmt.c_str(), result, args...);
    return result;
  }

 private:
  struct impl {
    template <typename... Args>
    static inline void format(const char* fmt, BaseString& result) {
      result += fmt;
    }

    template <typename... Args, typename V>
    static inline void format(const char* fmt, BaseString& result, V& value, Args&&... args) {
      for (size_t i = 0; fmt[i]; i++) {
        switch (fmt[i]) {
          case '\\':
            switch (fmt[++i]) {
              case 'n':
                result += '\n';
                break;
              case 't':
                result += '\t';
                break;
              case '0':
                result += '\0';
                break;
              case '{':
                result += '{';
                break;
              default:
                result += fmt[i];
            }
            break;
          case '{': {
            BaseString fmtString;
            i += 1;
            while (fmt[i] && fmt[i] != '}') fmtString += fmt[i++];
            // TODO: formats
            result += toString(value);
            impl::format(fmt+i+1, result, args...);
            return;
          }
          default:
            result += fmt[i];
        }
      }
    }

  }; /* struct impl */

};

template <typename T, typename CharT = char>
inline BaseString<CharT> toString(T value) {
  return value.toString();
}

template <typename CharT = char>
inline BaseString<CharT> toString(BaseString<CharT>& value) {
  return value;
}

template <typename CharT = char>
inline BaseString<CharT> toString(int value) {
  return BaseString(std::to_string(value));
}

template <typename CharT = char>
inline BaseString<CharT> toString(long value) {
  return BaseString(std::to_string(value));
}

template <typename CharT = char>
inline BaseString<CharT> toString(long long value) {
  return BaseString(std::to_string(value));
}

template <typename CharT = char>
inline BaseString<CharT> toString(unsigned value) {
  return BaseString(std::to_string(value));
}

template <typename CharT = char>
inline BaseString<CharT> toString(unsigned long value) {
  return BaseString(std::to_string(value));
}

template <typename CharT = char>
inline BaseString<CharT> toString(unsigned long long value) {
  return BaseString(std::to_string(value));
}

template <typename CharT = char>
inline BaseString<CharT> toString(float value) {
  return BaseString(std::to_string(value));
}

template <typename CharT = char>
inline BaseString<CharT> toString(double value) {
  return BaseString(std::to_string(value));
}

template <typename CharT = char>
inline BaseString<CharT> toString(void* value) {
  char buffer[64] = {0};
  snprintf(buffer, sizeof(buffer), "%p", value);
  return BaseString(buffer);
}

template <typename T1, typename T2, typename CharT = char>
inline BaseString<CharT> toString(const Pair<T1, T2>& value) {
  return BaseString<CharT>::format("({}, {})", value._1, value._2);
}

template <typename T, Collection<T> C, typename CharT = char>
inline BaseString<CharT> collectionToBaseString(const C& value) {
  BaseString<CharT> result = "{";
  for (size_t i = 0; i < value.size(); i++) {
    result.append(toString(value[i]));
    if (i + 1 < value.size())
      result.append(", ");
  }
  return result + "}";
}

template <typename T, typename CharT = char>
inline BaseString<CharT> toString(const Array<T>& value) {
  BaseString<CharT> result = "{";
  for (size_t i = 0; i < value.size(); i++) {
    result.append(toString(value[i]));
    if (i + 1 < value.size())
      result.append(", ");
  }
  return result + "}";
}

template <typename T, typename CharT = char>
inline BaseString<CharT> toString(const List<T>& value) {
  BaseString<CharT> result = "{";
  size_t index = 0;
  for (auto it = value.cbegin(); it != value.cend(); it++) {
    result.append(toString(*it));
    if (index + 1 < value.size())
      result.append(", ");
    index++;
  }
  return result + "}";
}

using String = BaseString<char>;

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_STRING_H_ */
