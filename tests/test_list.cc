#include "test.h"
#include <mrt/list.h>
#include <mrt/string.h>
#include <cstdio>


bool test_copy() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};
  mrt::List<int> arr2 = arr;

  return arr2 == arr;
}

bool test_append() {
  mrt::List<int> arr;
  mrt::List<int> expected = {0, 1, 2, 3, 4};
  
  for (int i = 0; i < 5; i++) {
    arr.append(i);
  }

  return arr == expected;
}

bool test_prepend() {
  mrt::List<int> arr;
  mrt::List<int> expected = {4, 3, 2, 1, 0};
  
  for (int i = 0; i < 5; i++) {
    arr.prepend(i);
  }

  return arr == expected;
}

bool test_pop() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};
  mrt::List<int> expected = {0, 1, 2, 3};

  int val = arr.pop();

  return val == 4 && arr == expected;
}

bool test_insert() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};
  mrt::List<int> expected = {0, 1, 2, 3};

  int val = arr.pop();

  return val == 4 && arr == expected;
}

bool test_remove() {
  mrt::List<int> arr = {0, 1, 4};
  mrt::List<int> expected = {0, 1, 2, 3, 4};
  
  arr.insert(2, 2);
  auto itr = arr.begin();
  for (int i = 0; i < 3; i++) itr++;
  arr.insert(itr, 3);

  return arr == expected;
}

bool test_clear() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};

  arr.clear();

  return arr.size() == 0;
}

bool test_contains() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};

  return arr.contains(3) && !arr.contains(8);
}

bool test_slice() {
  mrt::List<int> arr = {0, 1, 2, 3, 4, 5};
  mrt::List<int> expected = {2, 3, 4};

  auto s = arr.slice(2, 5);

  return arr.slice(2, 5) == expected;
}

bool test_find() {
  mrt::List<int> arr = {0, 1, 2, 3, 1, 5, 6, 1};
  mrt::Array<size_t> expected = {1, 4, 7};

  return arr.find(1) == expected;
}

bool test_lfind() {
  mrt::List<int> arr = {0, 1, 2, 3, 1, 5, 6, 1};

  return arr.lfind(1) == 1;
}

bool test_rfind() {
  mrt::List<int> arr = {0, 1, 2, 3, 1, 5, 6, 1};

  return arr.rfind(1) == 7;
}

bool test_unique() {
  mrt::List<int> arr = {0, 1, 2, 3, 1, 5, 6, 1};
  mrt::List<int> expected = {0, 1, 2, 3, 5, 6};

  return arr.unique() == expected;
}

bool test_foreach() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};
  mrt::List<int> result;

  arr.foreach([&result](auto x) { result.append(x); });

  return arr == result;
}

bool test_filter() {
  mrt::List<int> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  mrt::List<int> expected = {5, 6, 7};

  return arr.filter([](auto x) { return x > 4 && x < 8; }) == expected;
}

bool test_reduce() {
  mrt::List<int> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  return arr.reduce<int>([](int s, auto x) { return s + x; }) == 45;
}

bool test_reduceRight() {
  mrt::List<int> arr = {5, 1};

  return arr.reduceRight<int>([](int s, auto x) { return x - s; }) == 4;
}

bool test_map() {
  mrt::List<int> arr = {1, 2, 3};
  mrt::List<std::string> expected = {"1", "2", "3"};

  return arr.map<std::string>([](auto x) { return std::to_string(x); }) == expected;
}

bool test_get() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};

  return arr.get(1, 0) == 1 && arr.get(10, 4) == 4;
}

bool test_equals() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};
  mrt::List<int> arr2 = {0, 1, 2, 3, 4};

  return arr == arr2;
}

bool test_notequals() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};
  mrt::List<int> arr2 = {0, 1, 2, 3, 5};

  return arr != arr2;
}

bool test_combine() {
  mrt::List<int> arr = {0, 1, 2};
  mrt::List<int> arr2 = {3, 4, 5};
  mrt::List<int> expected = {0, 1, 2, 3, 4, 5};

  return (arr + arr2) == expected;
}

bool test_and() {
  mrt::List<int> arr = {10, 1, 2, 3, 4};
  mrt::List<int> arr2 = {15, 3, 1, 5, 2};
  mrt::List<int> expected = {1, 2, 3};

  return (arr & arr2) == expected;
}

bool test_or() {
  mrt::List<int> arr = {10, 1, 2, 3, 4};
  mrt::List<int> arr2 = {15, 3, 1, 5, 2};
  mrt::List<int> expected = {10, 4};

  return (arr | arr2) == expected;
}

bool test_iterators() {
  mrt::List<int> arr = {0, 1, 2, 3, 4};

  size_t index = 0;
  for (auto it = arr.begin(); it != arr.end(); it++) {
    if (*it != arr[index]) return false;
    index++;
  }

  return true;
}

int main(int argc, char ** argv) {
  mrt::TestFramework framework("list");

  framework.addTests({
    {"test_copy", test_copy},
    {"test_append", test_append},
    {"test_prepend", test_prepend},
    {"test_pop", test_pop},
    {"test_insert", test_insert},
    {"test_remove", test_remove},
    {"test_clear", test_clear},
    {"test_contains", test_contains},
    {"test_slice", test_slice},
    {"test_find", test_find},
    {"test_lfind", test_lfind},
    {"test_rfind", test_rfind},
    {"test_unique", test_unique},
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
