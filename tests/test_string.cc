#include "test.h"
#include <mrt/string.h>
#include <cstdio>

bool test_create_from_cstr() {
  mrt::String s = "ABC";

  return s.size() == 3 && s[0] == 'A' && s[1] == 'B' && s[2] == 'C';
}

bool test_create_from_cstr_len() {
  auto s = mrt::String("ABCEDF", 3);

  return s.size() == 3 && s[0] == 'A' && s[1] == 'B' && s[2] == 'C';
}

bool test_create_from_cppstr() {
  mrt::String s = std::string("ABC");

  return s.size() == 3 && s[0] == 'A' && s[1] == 'B' && s[2] == 'C';
}

bool test_cstr() {
  mrt::String s = "ABC";

  return s.c_str()[0] == 'A';
}

bool test_cppstr() {
  mrt::String s = "ABC";

  return s.cpp_str()[0] == 'A';
}

bool test_append_str() {
  mrt::String s = "ABC";

  s.append(mrt::String("DEF"));

  return s == "ABCDEF";
}

bool test_append_cstr() {
  mrt::String s = "ABC";

  s.append("DEF");

  return s == "ABCDEF";
}

bool test_append_cppstr() {
  mrt::String s = "ABC";

  s.append(std::string("DEF"));

  return s == "ABCDEF";
}

bool test_plus_str() {
  mrt::String s = "ABC";

  return (s + mrt::String("DEF")) == "ABCDEF";
}

bool test_plus_cstr() {
  mrt::String s = "ABC";

  return (s + "DEF") == "ABCDEF";
}

bool test_plus_cppstr() {
  mrt::String s = "ABC";

  return (s + std::string("DEF")) == "ABCDEF";
}

bool test_plus_char() {
  mrt::String s = "ABC";

  return (s + 'D') == "ABCD";
}

bool test_insert() {
  mrt::String s = "ABE";

  s.insert(2, "CD");

  return s == "ABCDE";
}

bool test_hash() {
  return mrt::String("abc").hash() != mrt::String("ABC").hash();
}

bool test_find() {
  mrt::String s = "ABCDEF";

  return s.find("DE") == 3;
}

bool test_substr() {
  mrt::String s = "ABCDEF";

  return s.substr(2, 5) == "CDE";
}

bool test_upper() {
  mrt::String s = "abc";

  return s.upper() == "ABC";
}

bool test_lower() {
  mrt::String s = "ABC";

  return s.lower() == "abc";
}

bool test_replace() {
  mrt::String s = "ABC123CDAFCA";

  return s.replace('A', '0') == "0BC123CD0FC0";
}

bool test_replace_str() {
  mrt::String s = "ABC CDE123 HGABC 091F";

  return s.replace("ABC", "123") == "123 CDE123 HG123 091F";
}

bool test_split() {
  mrt::String s = "abc cde 1234 ,,?";
  mrt::Array<mrt::String> expected = {"abc", "cde", "1234", ",,?"};
  auto result = s.split();

  return result == expected;
}

bool test_startsWith() {
  mrt::String s = "ABCDEFGH";

  return s.startsWith("ABC") && !s.startsWith("BCD");
}

bool test_endsWith() {
  mrt::String s = "ABCDEFGH";

  return s.endsWith("FGH") && !s.endsWith("EFG");
}

int main(int argc, char ** argv) {
  mrt::TestFramework framework("string");

  framework.addTests({
    {"test_create_from_cstr", test_create_from_cstr},
    {"test_create_from_cstr_len", test_create_from_cstr_len},
    {"test_create_from_cppstr", test_create_from_cppstr},
    {"test_cstr", test_cstr},
    {"test_cppstr", test_cppstr},
    {"test_append_str", test_append_str},
    {"test_append_cstr", test_append_cstr},
    {"test_append_cppstr", test_append_cppstr},
    {"test_plus_str", test_plus_str},
    {"test_plus_cstr", test_plus_cstr},
    {"test_plus_cppstr", test_plus_cppstr},
    {"test_plus_char", test_plus_char},
    {"test_insert", test_insert},
    {"test_hash", test_hash},
    {"test_find", test_find},
    {"test_substr", test_substr},
    {"test_upper", test_upper},
    {"test_lower", test_lower},
    {"test_replace", test_replace},
    {"test_replace_str", test_replace_str},
    {"test_split", test_split},
    {"test_startsWith", test_startsWith},
    {"test_endsWith", test_endsWith},
  });

  return framework.run(argc, argv);
}
