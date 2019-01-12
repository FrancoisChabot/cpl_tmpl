#ifndef CPL_TMPL_CONFIG_H
#define CPL_TMPL_CONFIG_H

#include <iostream>

// This file contains various user-configurable settings for the library.
// config.h needs to be consistent between library building and usage.

namespace cpl_tmpl {
// By default, cpl_tmpl expects std::ostream for the data destination, but
// you may substitute any type that supports operator<<(const T&) operations.
using Stream_t = std::ostream;
}  // namespace cpl_tmpl

#endif