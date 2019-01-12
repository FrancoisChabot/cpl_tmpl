#ifndef CPL_TMPL_VERSION_H
#define CPL_TMPL_VERSION_H

#include <iostream>

// This file contains various user-configurable settings for the library.
// config.h needs to be consistent between library building and usage.

namespace cpl_tmpl {
// By default, cpl_tmpl expects std::ostream for the data destination, but
// you may substitute any type that supports operator<<(const T&) operations.
constexpr unsigned int major_version = 0;
constexpr unsigned int minor_version = 1;
constexpr unsigned int patch_version = 0;

constexpr unsigned int version =
    major_version * 10000 + minor_version * 100 + patch_version;
}  // namespace cpl_tmpl

#endif