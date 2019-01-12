#ifndef CPL_IMPL_OPS_H
#define CPL_IMPL_OPS_H

#include "cpl_tmpl/interp/ast.h"
#include "cpl_tmpl/interp/template.h"

namespace cpl_tmpl {

class Print_raw_op : public Template_op {
 public:
  Print_raw_op(std::string_view data);
  void render(Stream_t& dst) const override;

 private:
  std::string_view data_;
};

class Print_op : public Template_op {
 public:
  Print_op(std::unique_ptr<ast::Expr> expr);
  void render(Stream_t& dst) const override;

 private:
  std::unique_ptr<ast::Expr> expr_;
};
}  // namespace cpl_tmpl

#endif