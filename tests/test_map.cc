#include "test.h"
#include <mrt/map.h>
#include <mrt/string.h>
#include <cstdio>

bool test_copy() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
  auto map2 = map;

  return map == map2;
}

bool test_set() {
  mrt::Map<mrt::String, int> map;

  map["a"] = 10;
  map["b"] = 20;

  return map["a"] == 10 && map["b"] == 20;
}

bool test_get() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};

  return map["a"] == 1 && map["b"] == 2 && map["c"] == 3;
}

bool test_remove() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
  mrt::Map<mrt::String, int> expected = {{"c", 3}};

  map.remove("b");
  map.remove("a");

  return map == expected;
}

bool test_clear() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};

  map.clear();

  return map.size() == 0 && map.capacity() == 0;
}

bool test_contains() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};

  return map.contains("a") && !map.contains("d");
}

bool test_foreach() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
  mrt::Map<mrt::String, int> result;

  map.foreach([&result](auto p) { result[p._1] = p._2; });

  return map == result;
}

bool test_filter() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}, {"f", 6}};
  mrt::Map<mrt::String, int> expected = {{"d", 4}, {"e", 5}};

  auto result = map.filter([](auto p) { return p._2 > 3 && p._2 < 6; });

  return result == expected;
}

bool test_reduce() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};

  auto result = map.reduce<int>([](int r, auto p) { return r + p._2; });

  return result == 6;
}

bool test_map() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
  mrt::Map<mrt::String, int> expected = {{"c", 11}, {"d", 12}, {"e", 13}};

  auto result = map.map<mrt::String, int>(
    [](const mrt::Pair<mrt::String, int>& p) -> mrt::Pair<mrt::String, int> {
      char buffer[2] = {0};
      buffer[0] = p._1[0] + 2;
      return {mrt::String(buffer), p._2 + 10};
    }
  );

  return result == expected;
}

bool test_equals() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
  mrt::Map<mrt::String, int> map2 = {{"b", 2}, {"a", 1}, {"c", 3}};

  return map == map2;
}

bool test_notequals() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
  mrt::Map<mrt::String, int> map2 = {{"b", 2}, {"a", 4}, {"d", 3}};

  return map != map2;
}

bool test_combine() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}};
  mrt::Map<mrt::String, int> map2 = {{"c", 3}, {"d", 4}};
  mrt::Map<mrt::String, int> expected = {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}};

  return (map + map2) == expected;
}

bool test_iterators() {
  mrt::Map<mrt::String, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
  mrt::Map<mrt::String, int> expected = {{"a", 1}, {"b", 2}, {"c", 3}};
  mrt::Map<mrt::String, int> result;

  for (auto [k, v] : map) {
    result[k] = v;
  }

  return result == expected;
}

int main(int argc, char ** argv) {
  mrt::TestFramework framework("map");

  framework.addTests({
    {"test_copy", test_copy},
    {"test_set", test_set},
    {"test_get", test_get},
    {"test_remove", test_remove},
    {"test_clear", test_clear},
    {"test_contains", test_contains},
    {"test_foreach", test_foreach},
    {"test_filter", test_filter},
    {"test_reduce", test_reduce},
    {"test_map", test_map},
    {"test_equals", test_equals},
    {"test_notequals", test_notequals},
    {"test_combine", test_combine},
    {"test_iterators", test_iterators},
  });

  return framework.run(argc, argv);
}
