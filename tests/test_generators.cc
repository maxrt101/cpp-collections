#include "test.h"
#include <mrt/map.h>
#include <mrt/array.h>
#include <mrt/string.h>
#include <mrt/generator.h>
#include <cstdio>

bool test_generator() {
  mrt::Array<int> expected = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  mrt::Array<int> result;

  for (auto x : mrt::Generator<int>([](size_t count) {
    if (count > 10) mrt::yield();
    return (int) count;
  })) {
    result.append(x);
  }

  return result == expected;
}

bool test_range() {
  mrt::Array<int> expected = {2, 4, 6, 8};
  mrt::Array<int> result;

  for (auto i : mrt::range(2, 10, 2)) {
    result.append(i);
  }

  return result == expected;
}

bool test_enumerate() {
  mrt::Array<mrt::String> sarr = {"abc", "cde", "xyz"};
  mrt::Map<size_t, mrt::String> expected = {{0, "abc"}, {1, "cde"}, {2, "xyz"}};
  mrt::Map<size_t, mrt::String> result;

  for (auto [i, p] : mrt::enumerate<mrt::String>(sarr)) {
    result[i] = p;
  }

  return result == expected;
}

bool test_zip() {
  mrt::Array<int> iarr {10, 20, 30};
  mrt::Array<mrt::String> sarr {"abc", "cde", "xyz"};
  auto expected = mrt::Map<int, mrt::String>::fromArrays(iarr, sarr);
  mrt::Map<int, mrt::String> result;

  for (auto [i, s] : mrt::zip<int, mrt::String>(iarr, sarr)) {
    result[i] = s;
  }

  return result == expected;
}

int main(int argc, char ** argv) {
  mrt::TestFramework framework("generators");

  framework.addTests({
    {"test_generator", test_generator},
    {"test_range", test_range},
    {"test_enumerate", test_enumerate},
    {"test_zip", test_zip}
  });

  return framework.run(argc, argv);
}
