#include "cpl_tmpl/interp/ops.h"
#include "cpl_tmpl/lex.h"
#include "cpl_tmpl/template.h"

#include <algorithm>
#include <cassert>
#include <functional>

namespace cpl_tmpl {
namespace {

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

  return result;
}

}  // namespace

std::unique_ptr<Template_op> handle_print_directive(
    std::string_view::iterator& ite, std::string_view::iterator end) {
  const std::string_view tag_end_str = "}}";
  std::advance(ite, 2);
  auto tag_end = std::search(
      ite, end,
      std::boyer_moore_searcher(tag_end_str.begin(), tag_end_str.end()));

  if (tag_end == end) {
    throw std::runtime_error("unterminated tag");
  }

  std::string_view expr_string(&*ite, tag_end - ite);
  ite = std::next(tag_end, 2);

  return std::make_unique<Print_op>(parse_expr(expr_string));
}

std::unique_ptr<Template_op> handle_scoped_directive(
    std::string_view::iterator& ite, std::string_view::iterator end) {
  throw std::runtime_error("no");
}

std::vector<std::unique_ptr<Template_op>> parse(std::string_view text) {
  using Ite = std::string_view::iterator;

  std::vector<std::unique_ptr<Template_op>> result;

  auto ite = text.begin();
  auto end = text.end();

  auto raw_start = text.begin();
  auto consume_raw = [&](Ite raw_end) {
    if (raw_end != raw_start) {
      result.emplace_back(std::make_unique<Print_raw_op>(
          std::string_view(&*raw_start, raw_end - raw_start)));
      raw_start = raw_end;
    }
  };

  while (true) {
    // Advance to the next open brace
    ite = std::find(ite, end, '{');

    if (ite == end) break;

    auto next = std::next(ite);
    // Handle the last character being an open brace
    if (ite == end) break;

    switch (*next) {
      case '{':
        consume_raw(ite);
        result.emplace_back(handle_print_directive(ite, end));
        raw_start = ite;
        break;
      case '%':
        consume_raw(ite);
        result.emplace_back(handle_scoped_directive(ite, end));
        raw_start = ite;
        break;
      default:
        ite = std::next(next);
        break;
    }
  }

  consume_raw(ite);
  return result;
}
}  // namespace cpl_tmpl