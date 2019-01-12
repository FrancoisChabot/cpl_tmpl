#include "cpl_tmpl/interp/ops.h"

namespace cpl_tmpl {
Print_raw_op::Print_raw_op(std::string_view data) : data_(std::move(data)) {}

void Print_raw_op::render(Stream_t& dst) const { dst << data_; }

Print_op::Print_op(std::unique_ptr<ast::Expr> expr) : expr_(std::move(expr)) {}

void Print_op::render(Stream_t& dst) const {
  auto evaled = expr_->eval();

  evaled.output(dst);
}
}  // namespace cpl_tmpl