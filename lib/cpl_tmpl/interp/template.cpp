#include "cpl_tmpl/interp/template.h"
#include <regex>
#include "cpl_tmpl/parse.h"

namespace cpl_tmpl {

Interp_template_impl::Interp_template_impl(std::string data)
    : data_(std::move(data)) {
  operations_ = parse(data_);
}

void Interp_template_impl::render(Stream_t& dst, const Context& ctx) const {
  for (const auto& op : operations_) {
    op->render(dst, ctx);
  }
}

// User-facing API
Interp_template::Interp_template(std::string data)
    : impl_(std::make_unique<Interp_template_impl>(data)) {}

Interp_template::~Interp_template() {}

void Interp_template::render(Stream_t& dst, const Context& ctx) const {
  impl_->render(dst, ctx);
}
}  // namespace cpl_tmpl