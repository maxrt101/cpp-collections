#ifndef _MRT_COLLECTIONS_TEST_H_
#define _MRT_COLLECTIONS_TEST_H_ 1

#include <functional>
#include <vector>
#include <string>
#include <cstring>

namespace mrt {

constexpr char COLOR_RESET[] = "\u001b[0m";
constexpr char COLOR_RED[]   = "\u001b[31m";
constexpr char COLOR_GREEN[] = "\u001b[32m";

class TestFramework {
 public:
  using TestFunction = std::function<bool()>;

  struct Test {
    std::string name;
    TestFunction fn;
  };

 public:
  inline TestFramework(const std::string& name) : m_name(name) {}

  inline ~TestFramework() = default;

  inline void addTest(const std::string& name, TestFunction fn) {
    m_tests.push_back({name, fn});
  }

  inline void addTests(std::vector<Test> tests) {
    for (auto& test : tests) {
      m_tests.push_back(test);
    }
  }

  inline int run(int argc, char ** argv) {
    if (argc > 1) {
      for (size_t i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
          printf(
            "mrt.collections Test Framework (test '%s')\n"
            "Usage: %s [OPTIONS]\n"
            "Options:\n"
            "  -c, --count  - Prints count of tests\n"
            "  -l, --list   - Prints list of tests\n"
            "  -q, --quiet  - Turns off output\n",
            m_name.c_str(), argv[0]
          );
          return 0;
        } else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--count")) {
          printf("%zu\n", m_tests.size());
          return 0;
        } else if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--list")) {
          for (auto& test : m_tests) {
            printf("%s ", test.name.c_str());
          }
          printf("\n");
        } else if (!strcmp(argv[i], "-q") || !strcmp(argv[i], "--quiet")) {
          m_isQuiet = true;
        } else {
          printf("%sERROR%s: Unknown argument\nType '%s -h' for usage\n", COLOR_RED, COLOR_RESET, argv[0]);
          return 1;
        }
      }
    }

    return runTests();
  }

 private:
  int runTests() {
    printf("Test '%s'\n", m_name.c_str());
    size_t succeded = 0;
    size_t failed = 0;
    for (auto& test : m_tests) {
      bool result = test.fn();
      if (result) {
        succeded++;
        if (!m_isQuiet) {
          printf("[  %sOK%s  ] %s\n", COLOR_GREEN, COLOR_RESET, test.name.c_str());
        }
      } else {
        failed++;
        if (!m_isQuiet) {
          printf("[ %sFAIL%s ] %s\n", COLOR_RED, COLOR_RESET, test.name.c_str());
        }
      }
    }
    
    if (!m_isQuiet) {
      printf(
        "Succeded: %s%d%s\n"
        "Failed:   %s%d%s\n",
        COLOR_GREEN, succeded, COLOR_RESET,
        COLOR_RED, failed, COLOR_RESET
      );
    }

    return failed;
  }

 private:
  bool m_isQuiet = false;
  std::string m_name;
  std::vector<Test> m_tests;
};

} /* namespace mrt */

#endif /* _MRT_COLLECTIONS_TEST_H_ */