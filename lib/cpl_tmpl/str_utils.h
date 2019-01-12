#ifndef CPL_TMPL_STR_UTILS_H
#define CPL_TMPL_STR_UTILS_H

#include "cpl_tmpl/config.h"

#include <algorithm>
#include <string_view>

namespace cpl_tmpl {

constexpr char* white_space = " \t\n\r\f\v\0";

// String trimming
constexpr std::string_view ltrim(std::string_view str) {
  auto found = str.find_first_not_of(white_space);
  return found == std::string_view::npos ? std::string_view{}
                                         : str.substr(found);
}

constexpr std::string_view rtrim(std::string_view str) {
  return str.substr(0, str.find_last_not_of(white_space) + 1);
}

constexpr std::string_view trim(std::string_view str) {
  return ltrim(rtrim(str));
}

// Returns the lenghts of the string that matches the predicate.
template <typename Unary>
std::string_view get_prefix(std::string_view data, Unary predicate) {
  auto end = std::find_if_not(data.begin(), data.end(), predicate);

  return {data.data(), std::size_t(end - data.begin())};
}

}  // namespace cpl_tmpl

#endif