#ifndef _MRT_COLLECTIONS_SORT_MERGE_H_
#define _MRT_COLLECTIONS_SORT_MERGE_H_ 1

#include <mrt/sort.h>

namespace mrt {

/*
  In-place merge sort, based on:
  Jyrki Katajainen, Tomi Pasanen, Jukka Teuhola. "Practical in-place mergesort". Nordic Journal of Computing, 1996.
*/
class MergeSort {

  template <typename T, Collection<T> C>
  void merge(C& collection, SortComparator<T> comparator, size_t start1, size_t end1, size_t start2, size_t end2, size_t dest) {
    while (start1 < end1 && start2 < end2) {
      swap<T>(collection, dest++, comparator(collection[start1], collection[start2]) ? start1++ : start2++);
    }

    while (start1 < end1) {
      swap<T>(collection, dest++, start1++);
    }

    while (start2 < end2) {
      swap<T>(collection, dest++, start2++);
    }
  }

  template <typename T, Collection<T> C>
  void sortSlice(C& collection, SortComparator<T> comparator, size_t start, size_t end, size_t dest) {
    size_t middle;
    
    if (end - start > 1) {
      middle = start + (end - start) / 2;
      sort(collection, comparator, start, middle);
      sort(collection, comparator, middle, end);
      merge(collection, comparator, start, middle, middle, end, dest);
    } else {
      while (start < end) {
        swap<T>(collection, start++, dest++);
      }
    }
  }

  template <typename T, Collection<T> C>
  void sort(C& collection, SortComparator<T> comparator, size_t start, size_t end) {
    size_t middle, n, dest;

    if (end - start > 1) {
      middle = start + (end - start) / 2;
      dest = start + end - middle;
      sortSlice(collection, comparator, start, middle, dest);

      while (dest - start > 2) {
        n = dest;
        dest = start + (n - start + 1) / 2;
        sortSlice(collection, comparator, dest, n, start);
        merge(collection, comparator, start, start + n - dest, n, end, dest);
      }

      for (size_t i = dest; i > start; i--) {
        for (size_t j = i; j < end && comparator(collection[j], collection[j-1]); j++) {
          swap<T>(collection, j, j - 1);
        }
      }
    }
  }

 public:
  template <typename T, Collection<T> C>
  inline void sort(SortComparator<T> comparator, C& collection) {
    sort(collection, comparator, 0, collection.size());
  }

};

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_SORT_MERGE_H_ */