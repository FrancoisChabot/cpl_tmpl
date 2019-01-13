#ifndef CPL_TMPL_TRAITS_H
#define CPL_TMPL_TRAITS_H

#include "cpl_tmpl/config.h"

#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cpl_tmpl {

template <typename T, typename Enable = void>
struct Data_traits {
  static constexpr bool is_sequence = false;
  static constexpr bool is_mapping = false;
  static constexpr bool is_value = true;

  static void print(const T& src, cpl_tmpl::Stream_t& dst) { dst << src; }
};

template <typename T>
struct Data_traits<std::vector<T>> {
  static constexpr bool is_sequence = true;
  static constexpr bool is_mapping = false;
  static constexpr bool is_value = false;
};

template <typename K, typename T>
struct Data_traits<
    std::map<K, T>,
    std::enable_if_t<std::is_constructible_v<std::string_view, K>>> {
  static constexpr bool is_sequence = false;
  static constexpr bool is_mapping = true;
  static constexpr bool is_value = false;

  static const T* lookup(const std::map<K, T>& src, std::string_view key) {
    auto found = src.find(K(key));
    if (found == src.end()) {
      return nullptr;
    }

    return &found->second;
  }
};

template <typename K, typename T>
struct Data_traits<
    std::unordered_map<K, T>,
    std::enable_if_t<std::is_constructible_v<std::string_view, K>>> {
  static constexpr bool is_sequence = false;
  static constexpr bool is_mapping = true;
  static constexpr bool is_value = false;

  static const T* lookup(const std::unordered_map<K, T>& src,
                         std::string_view key) {
    auto found = src.find(K(key));
    if (found == src.end()) {
      return nullptr;
    }

    return &found->second;
  }
};

}  // namespace cpl_tmpl
#endif