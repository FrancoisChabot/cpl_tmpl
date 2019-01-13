#ifndef CPL_TMPL_TEMPLATE_H
#define CPL_TMPL_TEMPLATE_H

#include "cpl_tmpl/config.h"
#include "cpl_tmpl/context.h"

#include <memory>
#include <string>

namespace cpl_tmpl {
class Template {
 public:
  virtual ~Template() {}
  virtual void render(Stream_t& dst, const Context& ctx) const = 0;
};

// A template that is interpreted on demand
class Interp_template_impl;
class Interp_template final : public Template {
 public:
  Interp_template(std::string data);
  ~Interp_template();

  // Renders the template to the destination stream
  void render(Stream_t& dst, const Context& ctx) const override;

 private:
  std::unique_ptr<Interp_template_impl> impl_;
};
}  // namespace cpl_tmpl
#endif