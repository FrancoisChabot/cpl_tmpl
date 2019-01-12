#include "cpl_tmpl/interp/ast.h"

namespace cpl_tmpl {

Value::Value(std::unique_ptr<Value_iface> impl) : impl_(std::move(impl)) {}

void Value::output(Stream_t& dst) const { impl_->output(dst); }

namespace ast {
String_literal::String_literal(std::string_view data)
    : data_(std::move(data)) {}

Value String_literal::eval() const { return make_value(data_); }

Id_ref::Id_ref(std::string_view init_id) : id_(init_id) {}

Value Id_ref::eval() const { return make_value(0); }
}  // namespace ast
}  // namespace cpl_tmpl
