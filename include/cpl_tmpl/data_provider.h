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
Data_provider data_provider(const T& v);

template <typename T, typename ParentT = Data_provider::Impl>
class Value_provider_impl : public ParentT {
 public:
  void get(const void* data, Stream_t& dst) const override {
    const T* as_t = reinterpret_cast<const T*>(data);
    Data_traits<T>::print(*as_t, dst);
  }
};

template <typename T, typename ParentT = Data_provider::Impl>
class Sequence_provider_impl : public ParentT {
 public:
  void visit(const void* data,
             std::function<void(Data_provider)> cb) const override {
    const T* as_t = reinterpret_cast<const T*>(data);

    for (const auto& i : *as_t) {
      cb(data_provider(i));
    }
  }
};

template <typename T, typename ParentT = Data_provider::Impl>
class Mapping_provider_impl : public ParentT {
 public:
  Data_provider get(const void* data, std::string_view key) const override {
    const T* as_t = reinterpret_cast<const T*>(data);

    return data_provider(*Data_traits<T>::lookup(*as_t, key));
  }
};

template <typename T, typename Enable = void>
struct get_provider_impl_t {
  using traits = Data_traits<T>;

  using with_value =
      std::conditional_t<traits::is_value, Value_provider_impl<T>,
                         Data_provider::Impl>;

  using with_sequence =
      std::conditional_t<traits::is_sequence,
                         Sequence_provider_impl<T, with_value>, with_value>;

  using with_mapping =
      std::conditional_t<traits::is_mapping,
                         Mapping_provider_impl<T, with_sequence>,
                         with_sequence>;

  using type = with_mapping;
};

template <typename T>
Data_provider data_provider(const T& v) {
  using provider_t = typename get_provider_impl_t<T>::type;
  static provider_t provider;
  return Data_provider(&v, &provider);
}

}  // namespace cpl_tmpl
#endif