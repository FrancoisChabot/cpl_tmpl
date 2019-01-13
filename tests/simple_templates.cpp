#include "catch.hpp"

#include "cpl_tmpl/data_provider.h"
#include "cpl_tmpl/template.h"

#include <sstream>

std::string render(std::string data,
                   const std::unordered_map<std::string, std::string>& env) {
  std::ostringstream dst;

  cpl_tmpl::Context ctx;
  for (const auto& e : env) {
    ctx.add_root(e.first, cpl_tmpl::data_provider(e.second));
  }

  cpl_tmpl::Interp_template(data).render(dst, ctx);
  return dst.str();
}

TEST_CASE("trivial templates", "[simple]") {
  std::unordered_map<std::string, std::string> ctx;
  REQUIRE(render("hello", ctx) == "hello");
  REQUIRE(render("hello world", ctx) == "hello world");
  REQUIRE(render("", ctx) == "");
  REQUIRE(render(" ", ctx) == " ");
  REQUIRE(render("  a b aa ", ctx) == "  a b aa ");
}

TEST_CASE("simple lookup", "[simple]") {
  std::unordered_map<std::string, std::string> ctx = {
      {"name", "joe"},
      {"age", "12"},
  };

  REQUIRE(render("{{name}}", ctx) == "joe");
  REQUIRE(render("His name is {{name}}!", ctx) == "His name is joe!");
  REQUIRE(render("{{name}} is {{age}} years old!", ctx) ==
          "joe is 12 years old!");
}
