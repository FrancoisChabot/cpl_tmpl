#include "cpl_tmpl/data_provider.h"

namespace cpl_tmpl {
void Data_provider::Impl::get(const void*, Stream_t&) const {}
void Data_provider::Impl::visit(const void*,
                                std::function<void(Data_provider)> cb) const {}
Data_provider Data_provider::Impl::get(const void*,
                                       std::string_view key) const {
  return Data_provider{nullptr, nullptr};
}
}  // namespace cpl_tmpl