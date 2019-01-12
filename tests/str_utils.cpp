#include "catch.hpp"

#include "cpl_tmpl/str_utils.h"

TEST_CASE("ltrim", "[lex]") {
  using cpl_tmpl::ltrim;

  REQUIRE(ltrim("") == "");
  REQUIRE(ltrim("aaa") == "aaa");
  REQUIRE(ltrim("aa ") == "aa ");
  REQUIRE(ltrim("aa  ") == "aa  ");
  REQUIRE(ltrim("  aa") == "aa");
}

TEST_CASE("trim", "[lex]") {
  using cpl_tmpl::trim;

  REQUIRE(trim("") == "");
  REQUIRE(trim("aaa") == "aaa");
  REQUIRE(trim("aa ") == "aa");
  REQUIRE(trim("aa  ") == "aa");
  REQUIRE(trim("  aa") == "aa");
}

TEST_CASE("rtrim", "[lex]") {
  using cpl_tmpl::rtrim;

  REQUIRE(rtrim("") == "");
  REQUIRE(rtrim("aaa") == "aaa");
  REQUIRE(rtrim("aa ") == "aa");
  REQUIRE(rtrim("aa  ") == "aa");
  REQUIRE(rtrim("  aa") == "  aa");
}
