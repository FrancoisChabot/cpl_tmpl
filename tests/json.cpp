#include "catch.hpp"

#include "cpl_tmpl/template.h"
#include "extern/json.hpp"

#include <sstream>

using json = nlohmann::json;

// Test integration with nlohmann/json

namespace cpl_tmpl {

template <>
struct Data_traits<json> {
  static constexpr bool is_sequence = false;
  static constexpr bool is_mapping = true;
  static constexpr bool is_value = true;

  // mapping support
  static const json* lookup(const json& src, std::string_view key) {
    auto found = src.find(key);
    if (found == src.end()) {
      return nullptr;
    }
    return &*found;
  }

  // Value support
  static void print(const nlohmann::json& src, cpl_tmpl::Stream_t& dst) {
    if (src.type() == json::value_t::string) {
      dst << src.get_ref<const json::string_t&>();
    } else {
      dst << src;
    }
  }
};
}  // namespace cpl_tmpl

namespace {
std::string render_to_string(const cpl_tmpl::Template& tmpl,
                             const cpl_tmpl::Context& ctx) {
  std::ostringstream dst;
  tmpl.render(dst, ctx);
  return dst.str();
}
}  // namespace

TEST_CASE("simple json usage", "[lex]") {
  nlohmann::json j;
  j["name"] = "joe";
  j["age"] = "12";

  cpl_tmpl::Interp_template tmpl("His name is {{person.name}}!");
  cpl_tmpl::Context ctx;

  ctx.add_root("person", j);

  REQUIRE(render_to_string(tmpl, ctx) == "His name is joe!");
}
