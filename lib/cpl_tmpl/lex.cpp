#include "cpl_tmpl/lex.h"
#include "cpl_tmpl/str_utils.h"

#include <cassert>
#include <iostream>
#include <optional>

namespace cpl_tmpl {
namespace {

bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_id_char(char c) { return is_alpha(c) || c == '_'; }

bool is_num(char c) { return (c >= '0' && c <= '9'); }
}  // namespace

Token get_head_token(std::string_view data) {
  assert(data.length() > 0);
  assert(data.find_first_of(white_space) != 0);

  auto try_tok = [&](std::string_view tok,
                     Token_t type) -> std::optional<Token> {
    if (data.length() >= tok.length() &&
        data.compare(0, tok.length(), tok) == 0) {
      return Token{type, data.substr(0, tok.length())};
    }
    return std::nullopt;
  };

  // suuuuuuuper lazy, we could use a trie instead, but we'll deal with this
  // if performance ever becomes an issue.
  if (auto t = try_tok("==", Token_t::eq_cmp)) return *t;
  if (auto t = try_tok("<=", Token_t::le_cmp)) return *t;
  if (auto t = try_tok(">=", Token_t::ge_cmp)) return *t;
  if (auto t = try_tok("!=", Token_t::neq_cmp)) return *t;

  if (auto t = try_tok("<", Token_t::lt_cmp)) return *t;
  if (auto t = try_tok(">", Token_t::gt_cmp)) return *t;

  if (auto t = try_tok("=", Token_t::eq)) return *t;
  if (auto t = try_tok(".", Token_t::period)) return *t;
  if (auto t = try_tok("(", Token_t::parens_open)) return *t;
  if (auto t = try_tok(")", Token_t::parens_close)) return *t;
  if (auto t = try_tok("-", Token_t::minus)) return *t;
  if (auto t = try_tok("+", Token_t::plus)) return *t;
  if (auto t = try_tok("*", Token_t::mul)) return *t;
  if (auto t = try_tok("/", Token_t::div)) return *t;

  if (is_id_char(data[0])) {
    return {Token_t::identifier, get_prefix(data, is_id_char)};
  }
  if (is_num(data[0])) {
    return {Token_t::number, get_prefix(data, is_num)};
  }

  throw std::runtime_error("tokenizer failure");
}

Token_iterator::Token_iterator(std::string_view data) : data_(data) {
  data_ = trim(data_);
  if (data_.length() == 0) {
    data_ = std::string_view();
  } else {
    token_ = get_head_token(data_);
  }
}

bool Token_iterator::operator==(const Token_iterator& rhs) const {
  return (data_.data() == rhs.data_.data()) &&
         (data_.length() == rhs.data_.length());
}

Token_iterator& Token_iterator::operator++() {
  *this = Token_iterator(data_.substr(token_.raw.length()));
  return *this;
}
}  // namespace cpl_tmpl
