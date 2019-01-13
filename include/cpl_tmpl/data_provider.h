#ifndef CPL_TMPL_DATA_PROVIDER_H
#define CPL_TMPL_DATA_PROVIDER_H

#include "cpl_tmpl/config.h"
#include "cpl_tmpl/traits.h"

#include <functional>
#include <string_view>
#include <type_traits>
#include <vector>

namespace cpl_tmpl {

class Data_provider_iface;

class Data_provider {
 public:
  class Impl {
   public:
    virtual void get(const void*, Stream_t&) const;
    virtual void visit(const void*,
                       std::function<void(Data_provider)> cb) const;
    virtual Data_provider get(const void*, std::string_view key) const;

   protected:
    ~Impl() = default;
  };

  Data_provider() = default;
  Data_provider(const void* data, Impl* impl) : data_(data), impl_(impl) {}
  Data_provider(const Data_provider&) = default;
  Data_provider& operator=(const Data_provider&) = default;

  void get(Stream_t& dst) const { return impl_->get(data_, dst); }

  void visit(std::function<void(Data_provider)> cb) const {
    impl_->visit(data_, std::move(cb));
  }

  Data_provider get(std::string_view key) const {
    return impl_->get(data_, key);
  }

 private:
  const void* data_ = nullptr;
  const Impl* impl_ = nullptr;
};

template <typename T>
auto get_provider_impl();

template <typename T>
class Data_provider_impl : public Data_provider::Impl {
 public:
  void get(const void* data, Stream_t& dst) const override {
    const T* as_t = reinterpret_cast<const T*>(data);
    dst << *as_t;
  }
};

template <typename IteT>
class Sequence_provider_impl : public Data_provider::Impl {
 public:
  void visit(const void* data,
             std::function<void(Data_provider)> cb) const override {
    const T* as_t = reinterpret_cast<const T*>(data);
    auto child_provider = get_provider_impl<T>();

    for (const auto& i : *as_t) {
      cb(Data_provider(&i, child_provider));
    }
  }
};

template <typename T>
class Mapping_provider_impl : public Data_provider::Impl {
 public:
  Data_provider get(const void* data, std::string_view key) const override {
    const T* as_t = reinterpret_cast<const T*>(data);

    using key_t = typename T::key_type;
    using val_t = typename T::mapped_type;

    auto found = as_t->find(key_t(key));

    if (found == as_t->end()) {
      return Data_provider();
    }

    auto child_provider = get_provider_impl<val_t>();
    return Data_provider(&found->second, child_provider);
  }
};

template <typename T>
auto get_provider_impl() {
  if constexpr (is_sequence_v<T>) {
    static Sequence_provider_impl<T> provider;
    return &provider;
  } else if constexpr (is_mapping_v<T>) {
    static Mapping_provider_impl<T> provider;
    return &provider;
  } else {
    static Data_provider_impl<T> provider;
    return &provider;
  }
}

template <typename T>
Data_provider data_provider(const T& v) {
  return Data_provider(&v, get_provider_impl<T>());
}

}  // namespace cpl_tmpl
#endif