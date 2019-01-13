#ifndef CPL_TMPL_INTERP_TEMPLATE_H
#define CPL_TMPL_INTERP_TEMPLATE_H

#include "cpl_tmpl/config.h"
#include "cpl_tmpl/context.h"

#include <memory>
#include <string>
#include <vector>

namespace cpl_tmpl {
class Template_op {
 public:
  virtual ~Template_op() {}
  virtual void render(Stream_t& dst, const Context& ctx) const = 0;
};

class Interp_template_impl {
 public:
  Interp_template_impl(std::string data);
  void render(Stream_t& dst, const Context& ctx) const;

 private:
  std::string data_;
  std::vector<std::unique_ptr<Template_op>> operations_;
};
}  // namespace cpl_tmpl
#endif