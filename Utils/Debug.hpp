#pragma once

namespace Utils
{
#ifdef NDBUG
constexpr bool is_target_debug = false;
#else
constexpr bool is_target_debug = true;
#endif
}
