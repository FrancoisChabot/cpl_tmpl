#include "cpl_tmpl/interp/ops.h"
#include "cpl_tmpl/lex.h"
#include "cpl_tmpl/template.h"

#include <cassert>
#include <regex>

namespace cpl_tmpl {
namespace {
const std::regex tag_regex(R"TAG(\{%(.*?(?=%\}))%}|\{\{(.*?(?=\}\}))\}\})TAG");

using Expr_ptr = std::unique_ptr<ast::Expr>;

Expr_ptr parse_expr(std::string_view raw);
Expr_ptr parse_expr(Token_iterator& b, Token_iterator e);
Expr_ptr parse_primary(Token_iterator& b, Token_iterator e);
Expr_ptr parse_parens(Token_iterator& b, Token_iterator e);

Expr_ptr parse_primary(Token_iterator& b, Token_iterator e) {
  switch (b->type) {
    case Token_t::identifier:
      return std::make_unique<ast::Id_ref>(b->raw);
      break;
    case Token_t::number:
      break;
    case Token_t::parens_open:
      return parse_parens(b, e);
      break;
    default:
      break;
  }
  return nullptr;
}

Expr_ptr parse_parens(Token_iterator& b, Token_iterator e) {
  ++b;
  auto result = parse_expr(b, e);
  if (b->type != Token_t::parens_close) {
    throw std::runtime_error("expecting )");
  }
  ++b;
  return result;
}

Expr_ptr parse_expr(std::string_view raw) {
  Token_iterator b(raw);

  return parse_expr(b, Token_iterator());
}

Expr_ptr parse_expr(Token_iterator& b, Token_iterator e) {
  if (b == e) {
    return nullptr;
  }

  auto result = parse_primary(b, e);

  return nullptr;
}

std::unique_ptr<Template_op> eval_match(std::string_view match) {
  assert(match.length() >= 4);
  std::unique_ptr<Template_op> result;

  switch (match[1]) {
    case '%':

      break;
    case '{':
      result = std::make_unique<Print_op>(
          parse_expr(match.substr(2, match.length() - 4)));
      break;
    default:
      assert(false);
      break;
  }
  return result;
}
}  // namespace

std::vector<std::unique_ptr<Template_op>> parse(std::string_view text) {
  using Ite = std::string_view::iterator;

  std::vector<std::unique_ptr<Template_op>> result;

  std::size_t last_match_end = 0;

  std::regex_iterator<Ite> ite(text.begin(), text.end(), tag_regex);
  std::regex_iterator<Ite> end;

  for (; ite != end; ++ite) {
    auto match_pos = ite->position();
    auto match_len = ite->length();

    // The amount of raw data between the last match and now.
    auto raw_prefix_len = match_pos - last_match_end;

    if (raw_prefix_len > 0) {
      auto prefix = text.substr(last_match_end, raw_prefix_len);
      result.emplace_back(std::make_unique<Print_raw_op>(prefix));
    }

    std::string_view match_view(text.data() + match_pos, match_len);
    auto evaled_match = eval_match(match_view);
    if (evaled_match) {
      result.emplace_back(std::move(evaled_match));
    }

    last_match_end += raw_prefix_len;
    last_match_end += match_len;
  }

  auto raw_suffix_len = text.length() - last_match_end;
  if (raw_suffix_len > 0) {
    auto suffix = text.substr(last_match_end, raw_suffix_len);
    result.emplace_back(std::make_unique<Print_raw_op>(suffix));
  }

  return result;
}
}  // namespace cpl_tmpl