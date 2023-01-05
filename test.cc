#include <type_traits>
#include <functional>
#include <concepts>
#include <string>
#include <cstdio>

#include <mrt/iterator.h>
#include <mrt/iterable.h>
#include <mrt/generator.h>
#include <mrt/array.h>
#include <mrt/pair.h>

void arrayTest() {
  mrt::Array<int> iarr {10, 20, 30};

  for (int i = 40; i < 200; i+=10) {
    iarr.append(i);
  }

  iarr.remove(5);

  printf("size: %zu\ncapacity: %zu\n", iarr.size(), iarr.capacity());
  for (auto x : iarr) {
    printf("%d\n", x);
  }
}

void generatorTest() {
  // for (auto x : mrt::Generator<int>([](size_t count) {
  //   if (count > 10) throw mrt::GeneratorException();
  //   return (int)count;
  // })) {
  //   printf("%d\n", x);
  // }

  mrt::Array<int> iarr;

  for (int i = 10; i <= 100; i+=10) {
    iarr.append(i);
  }

  // for (auto [i, x] : mrt::Generator<mrt::Pair<size_t, int>>([&iarr](size_t count) -> mrt::Pair<size_t, int> {
  //   if (count >= iarr.size()) throw mrt::GeneratorException();
  //   return {count, iarr[count]};
  // })) {
  //   printf("%zu -> %d\n", i, x);
  // }

  for (auto [i, x] : mrt::enumerate<int>(iarr)) {
    printf("%zu -> %d\n", i, x);
  }
}

void rangeTest() {
  for (auto i : mrt::range(2, 10, 2)) {
    printf("%zu\n", i);
  }
}

void zipTest() {
  mrt::Array<int> iarr {10, 20, 30, 40, 50, 60};
  mrt::Array<std::string> sarr {"abc", "cde", "xyz"};

  for (auto [i, s] : mrt::zip<int, std::string>(iarr, sarr)) {
    printf("%d -> '%s'\n", i, s.c_str());
  }
}

void enumerateZipTest() {
  mrt::Array<int> iarr {10, 20, 30, 40, 50, 60};
  mrt::Array<std::string> sarr {"abc", "cde", "xyz"};

  for (auto [i, p] : mrt::enumerate<mrt::Pair<int, std::string>>(mrt::zip<int, std::string>(iarr, sarr))) {
    printf("%d -> (%d, '%s')\n", i, p._1, p._2.c_str());
  }
}

template <typename I, typename T> requires mrt::Iterable<I, T>
void testIterable() {}

int main() {
  testIterable<mrt::Generator<int>, int>();

  arrayTest();
  generatorTest();
  rangeTest();
  zipTest();
  enumerateZipTest();

  return 0;
}
