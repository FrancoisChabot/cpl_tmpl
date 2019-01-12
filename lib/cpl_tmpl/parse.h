#ifndef CPL_IMPL_PARSE_H
#define CPL_IMPL_PARSE_H

#include "cpl_tmpl/interp/ops.h"

#include <string_view>
#include <vector>

namespace cpl_tmpl {
std::vector<std::unique_ptr<Template_op>> parse(std::string_view);
}

#endif