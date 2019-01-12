#ifndef CPL_IMPL_AST_H
#define CPL_IMPL_AST_H

#include "cpl_tmpl/template.h"

#include <string_view>

namespace cpl_tmpl {
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

namespace ast {
class Expr {
 public:
  virtual ~Expr() {}
  virtual Value eval() const = 0;
};

class Id_ref : public Expr {
 public:
  Id_ref(std::string_view);

  virtual ~Id_ref() {}
  virtual Value eval() const override;

 private:
  std::string_view id_;
};

class String_literal : public Expr {
 public:
  String_literal(std::string_view data);
  Value eval() const override;

 private:
  std::string_view data_;
};
}  // namespace ast
}  // namespace cpl_tmpl

#endif