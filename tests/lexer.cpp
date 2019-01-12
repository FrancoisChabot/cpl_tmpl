#include "catch.hpp"

#include "cpl_tmpl/lex.h"

TEST_CASE("empty lexing operation", "[lex]") {
  auto b = cpl_tmpl::Token_iterator("");
  auto e = cpl_tmpl::Token_iterator();

  REQUIRE(b == e);
}

TEST_CASE("simple lexing", "[lex]") {
  auto b = cpl_tmpl::Token_iterator("aa+bb=cc");
  auto e = cpl_tmpl::Token_iterator();

  REQUIRE(b->raw == "aa");
  ++b;
  REQUIRE(b->raw == "+");
  ++b;
  REQUIRE(b->raw == "bb");
  ++b;
  REQUIRE(b->raw == "=");
  ++b;
  REQUIRE(b->raw == "cc");
  ++b;
  REQUIRE(b == e);
}
