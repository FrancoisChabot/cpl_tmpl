#ifndef CPL_TMPL_LEX_H
#define CPL_TMPL_LEX_H

#include "cpl_tmpl/config.h"

namespace cpl_tmpl {
enum class Token_t {
  unknown,

  identifier,
  number,

  parens_open,
  parens_close,

  eq_cmp,
  le_cmp,
  ge_cmp,
  neq_cmp,
  lt_cmp,
  gt_cmp,

  eq,
  period,
  minus,
  plus,
  mul,
  div,
};

struct Token {
  Token_t type = Token_t::unknown;
  std::string_view raw;
};

struct Token_iterator {
  Token_iterator() = default;
  Token_iterator(std::string_view data);
  Token_iterator(const Token_iterator&) = default;

  const Token& operator*() const { return token_; }
  const Token* operator->() const { return &token_; }

  Token_iterator& operator++();
  bool operator==(const Token_iterator& rhs) const;
  bool operator!=(const Token_iterator& rhs) const;

  Token_iterator& operator=(const Token_iterator&) = default;

 private:
  std::string_view data_;
  Token token_;
};
}  // namespace cpl_tmpl

#endif