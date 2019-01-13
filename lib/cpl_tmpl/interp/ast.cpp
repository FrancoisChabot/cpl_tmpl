#include "cpl_tmpl/interp/ast.h"

namespace cpl_tmpl {
/*
Value::Value(std::unique_ptr<Value_iface> impl) : impl_(std::move(impl)) {}

void Value::output(Stream_t& dst) const { impl_->output(dst); }
*/
namespace ast {
String_literal::String_literal(std::string_view data)
    : data_(std::move(data)) {}

Data_provider String_literal::eval(const Context&) const {
  return data_provider(data_);
}

Id_ref::Id_ref(std::string_view init_id) : id_(init_id) {}

Data_provider Id_ref::eval(const Context& ctx) const {
  auto found = ctx.lookup(std::string(id_));
  if (!found) {
    return data_provider("");
  }

  return *found;
}

Resolve::Resolve(Expr_ptr lhs, std::string_view init_id)
    : lhs_(std::move(lhs)), key_(init_id) {}

Data_provider Resolve::eval(const Context& ctx) const {
  return lhs_->eval(ctx).get(key_);
}

}  // namespace ast
}  // namespace cpl_tmpl
