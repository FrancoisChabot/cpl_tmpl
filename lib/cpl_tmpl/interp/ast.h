#ifndef CPL_IMPL_AST_H
#define CPL_IMPL_AST_H

#include "cpl_tmpl/data_provider.h"
#include "cpl_tmpl/template.h"

#include <string_view>

namespace cpl_tmpl {
/*
class Value_iface {
public:
virtual ~Value_iface() {}
virtual void output(Stream_t& dst) const = 0;
};

class Value {
public:
Value(std::unique_ptr<Value_iface>);

void output(Stream_t& dst) const;

private:
std::unique_ptr<Value_iface> impl_;
};

template <typename T>
class Value_impl : public Value_iface {
public:
Value_impl(T v) : val_(std::move(v)) {}

void output(Stream_t& dst) const override { dst << val_; }

private:
T val_;
};

template <typename T>
Value make_value(T v) {
return Value(std::make_unique<Value_impl<T>>(std::move(v)));
}
*/

namespace ast {
class Expr {
 public:
  virtual ~Expr() {}
  virtual Data_provider eval(const Context& ctx) const = 0;
};

using Expr_ptr = std::unique_ptr<Expr>;

// Identifier
class Id_ref final : public Expr {
 public:
  Id_ref(std::string_view);

  Data_provider eval(const Context& ctx) const override;

 private:
  std::string_view id_;
};

// <Expr>.Identifier
class Resolve final : public Expr {
 public:
  Resolve(Expr_ptr, std::string_view);
  Data_provider eval(const Context& ctx) const override;

 private:
  Expr_ptr lhs_;
  std::string_view key_;
};

// <Expr> |Identifier
class Filter final : public Expr {};

class String_literal final : public Expr {
 public:
  String_literal(std::string_view data);
  Data_provider eval(const Context& ctx) const override;

 private:
  std::string_view data_;
};
}  // namespace ast
}  // namespace cpl_tmpl

#endif