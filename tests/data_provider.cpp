#include "catch.hpp"

#include "cpl_tmpl/data_provider.h"

#include <sstream>

std::string get(cpl_tmpl::Data_provider dp) {
  std::ostringstream dst;
  dp.get(dst);
  return dst.str();
}

TEST_CASE("Numeric", "[data_provider]") {
  REQUIRE(get(cpl_tmpl::data_provider(3)) == "3");
}

TEST_CASE("String", "[data_provider]") {
  std::string data = "abc";
  REQUIRE(get(cpl_tmpl::data_provider(data)) == "abc");
}

TEST_CASE("Lookup", "[data_provider]") {
  using map_t = std::map<std::string, std::string>;
  std::map<std::string, std::string> data = {{"a", "b"}};

  REQUIRE(get(cpl_tmpl::data_provider(data).get("a")) == "b");
}