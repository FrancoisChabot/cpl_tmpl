#ifndef CPL_TMPL_CONTEXT_H
#define CPL_TMPL_CONTEXT_H

#include "cpl_tmpl/config.h"
#include "cpl_tmpl/data_provider.h"

#include <optional>
#include <unordered_map>

namespace cpl_tmpl {
class Context {
 public:
  std::optional<Data_provider> lookup(const std::string& name) const {
    auto found = data_roots_.find(name);
    if (found == data_roots_.end()) {
      return std::nullopt;
    }

    return found->second;
  }

  template <typename T>
  void add_root(std::string key, const T& data) {
    add_root_provider(key, data_provider(data));
  }

  void add_root_provider(std::string key, Data_provider data) {
    data_roots_.emplace(std::move(key), data);
  }

 private:
  std::unordered_map<std::string, Data_provider> data_roots_;
};
}  // namespace cpl_tmpl
#endif