#include "test.h"
#include <mrt/array.h>
#include <cstdio>

bool test_copy() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};
  mrt::Array<int> arr2 = arr;

  return arr2 == arr;
}

bool test_append() {
  mrt::Array<int> arr;
  mrt::Array<int> expected = {0, 1, 2, 3, 4};
  
  for (int i = 0; i < 5; i++) {
    arr.append(i);
  }

  return arr == expected;
}

bool test_prepend() {
  mrt::Array<int> arr;
  mrt::Array<int> expected = {4, 3, 2, 1, 0};
  
  for (int i = 0; i < 5; i++) {
    arr.prepend(i);
  }

  return arr == expected;
}

bool test_pop() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};
  mrt::Array<int> expected = {0, 1, 2, 3};

  int val = arr.pop();

  return val == 4 && arr == expected;
}

bool test_insert() {
  mrt::Array<int> arr = {0, 1, 4};
  mrt::Array<int> expected = {0, 1, 2, 3, 4};
  
  arr.insert(2, 2);
  arr.insert(arr.begin() + 3, 3);

  return arr == expected;
}

bool test_remove() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};
  mrt::Array<int> expected = {0, 1, 4};
  
  arr.remove(arr.begin() + 3);
  arr.remove(2);

  return arr == expected;
}

bool test_remove_many() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};
  mrt::Array<int> expected = {0, 1, 4};
  
  arr.remove(2, 4);

  return arr == expected;
}

bool test_resize() {
  mrt::Array<int> arr;
  mrt::Array<int> expected = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
  
  for (int i = 0; i < 20; i++) {
    arr.append(i);
  }

  return arr == expected;
}

bool test_reserve() {
  mrt::Array<int> arr;

  arr.reserve(100);

  return arr.size() == 0 && arr.capacity() == 100;
}

bool test_clear() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};

  arr.clear();

  return arr.size() == 0 && arr.capacity() == 0 && arr.data() == nullptr;
}

bool test_empty() {
  auto arr = mrt::Array<int>::empty(16);

  return arr.size() == 0 && arr.capacity() == 16;
}

bool test_filled() {
  auto arr = mrt::Array<int>::filled(16, 1);
  mrt::Array<int> expected = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  return arr.size() == 16 && arr == expected;
}

bool test_contains() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};

  return arr.contains(3) && !arr.contains(8);
}

bool test_slice() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4, 5};
  mrt::Array<int> expected = {2, 3, 4};

  return arr.slice(2, 5) == expected;
}

bool test_find() {
  mrt::Array<int> arr = {0, 1, 2, 3, 1, 5, 6, 1};
  mrt::Array<size_t> expected = {1, 4, 7};

  return arr.find(1) == expected;
}

bool test_lfind() {
  mrt::Array<int> arr = {0, 1, 2, 3, 1, 5, 6, 1};

  return arr.lfind(1) == 1;
}

bool test_rfind() {
  mrt::Array<int> arr = {0, 1, 2, 3, 1, 5, 6, 1};

  return arr.rfind(1) == 7;
}

bool test_unique() {
  mrt::Array<int> arr = {0, 1, 2, 3, 1, 5, 6, 1};
  mrt::Array<int> expected = {0, 1, 2, 3, 5, 6};

  return arr.unique() == expected;
}

bool test_reverse() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};
  mrt::Array<int> expected = {4, 3, 2, 1, 0};
  mrt::Array<int> arr_rev = arr;
  arr_rev.reverse();

  return arr.reversed() == expected && arr_rev == expected;
}

bool test_sort_asc() {
  mrt::Array<int> arr = {1, 10, 1941, 13, 3, -6, 14};
  mrt::Array<int> expected = {-6, 1, 3, 10, 13, 14, 1941};

  arr.sort(mrt::asc<int>);

  return arr == expected;
}

bool test_sort_desc() {
  mrt::Array<int> arr = {1, 10, 1941, 13, 3, -6, 14};
  mrt::Array<int> expected = {1941, 14, 13, 10, 3, 1, -6};

  arr.sort(mrt::desc<int>);

  return arr == expected;
}

bool test_foreach() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};
  mrt::Array<int> result;

  arr.foreach([&result](auto x) { result.append(x); });

  return arr == result;
}

bool test_filter() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  mrt::Array<int> expected = {5, 6, 7};

  return arr.filter([](auto x) { return x > 4 && x < 8; }) == expected;
}

bool test_reduce() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  return arr.reduce<int>([](int s, auto x) { return s + x; }) == 45;
}

bool test_reduceRight() {
  mrt::Array<int> arr = {5, 1};

  return arr.reduceRight<int>([](int s, auto x) { return x - s; }) == 4;
}

bool test_map() {
  mrt::Array<int> arr = {1, 2, 3};
  mrt::Array<std::string> expected = {"1", "2", "3"};

  return arr.map<std::string>([](auto x) { return std::to_string(x); }) == expected;
}

bool test_get() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};

  return arr.get(1, 0) == 1 && arr.get(10, 4) == 4;
}

bool test_equals() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};
  mrt::Array<int> arr2 = {0, 1, 2, 3, 4};

  return arr == arr2;
}

bool test_notequals() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};
  mrt::Array<int> arr2 = {0, 1, 2, 3, 5};

  return arr != arr2;
}

bool test_combine() {
  mrt::Array<int> arr = {0, 1, 2};
  mrt::Array<int> arr2 = {3, 4, 5};
  mrt::Array<int> expected = {0, 1, 2, 3, 4, 5};

  return (arr + arr2) == expected;
}

bool test_and() {
  mrt::Array<int> arr = {10, 1, 2, 3, 4};
  mrt::Array<int> arr2 = {15, 3, 1, 5, 2};
  mrt::Array<int> expected = {1, 2, 3};

  return (arr & arr2) == expected;
}

bool test_or() {
  mrt::Array<int> arr = {10, 1, 2, 3, 4};
  mrt::Array<int> arr2 = {15, 3, 1, 5, 2};
  mrt::Array<int> expected = {10, 4};

  return (arr | arr2) == expected;
}

bool test_iterators() {
  mrt::Array<int> arr = {0, 1, 2, 3, 4};

  size_t index = 0;
  for (auto it = arr.begin(); it != arr.end(); it++) {
    if (*it != arr[index] || it.index() != index) return false;
    index++;
  }

  return true;
}

int main(int argc, char ** argv) {
  mrt::TestFramework framework("array");

  framework.addTests({
    {"test_copy", test_copy},
    {"test_append", test_append},
    {"test_prepend", test_prepend},
    {"test_pop", test_pop},
    {"test_insert", test_insert},
    {"test_remove", test_remove},
    {"test_remove_many", test_remove_many},
    {"test_resize", test_resize},
    {"test_reserve", test_reserve},
    {"test_clear", test_clear},
    {"test_empty", test_empty},
    {"test_filled", test_filled},
    {"test_contains", test_contains},
    {"test_slice", test_slice},
    {"test_find", test_find},
    {"test_lfind", test_lfind},
    {"test_rfind", test_rfind},
    {"test_unique", test_unique},
    {"test_reverse", test_reverse},
    {"test_sort_asc", test_sort_asc},
    {"test_sort_desc", test_sort_desc},
    {"test_foreach", test_foreach},
    {"test_filter", test_filter},
    {"test_reduce", test_reduce},
    {"test_reduceRight", test_reduceRight},
    {"test_map", test_map},
    {"test_get", test_get},
    {"test_equals", test_equals},
    {"test_notequals", test_notequals},
    {"test_combine", test_combine},
    {"test_and", test_and},
    {"test_or", test_or},
    {"test_iterators", test_iterators},
  });

  return framework.run(argc, argv);
}
