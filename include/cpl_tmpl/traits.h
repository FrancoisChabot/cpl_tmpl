#ifndef CPL_TMPL_TRAITS_H
#define CPL_TMPL_TRAITS_H

#include "cpl_tmpl/config.h"

#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cpl_tmpl {

template <typename T>
struct is_sequence_t : public std::false_type {};

template <typename T>
struct is_sequence_t<std::vector<T>> : public std::true_type {};

template <typename T>
struct is_mapping_t : public std::false_type {};

template <typename T>
struct is_mapping_t<std::map<std::string, T>> : public std::true_type {};

template <typename T>
struct is_mapping_t<std::map<std::string_view, T>> : public std::true_type {};

template <typename T>
struct is_mapping_t<std::unordered_map<std::string, T>>
    : public std::true_type {};

template <typename T>
struct is_mapping_t<std::unordered_map<std::string_view, T>>
    : public std::true_type {};

template <typename T>
constexpr bool is_sequence_v = is_sequence_t<T>::value;

template <typename T>
constexpr bool is_mapping_v = is_mapping_t<T>::value;
}  // namespace cpl_tmpl
#endif