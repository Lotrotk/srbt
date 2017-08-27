#pragma once

#include <array>
#include <functional>

namespace Utils
{
	template<typename T, typename... Args>
    std::array<std::reference_wrapper<T>, sizeof...(Args) > ref_array(Args&... inArgs)
    {
		return std::array<std::reference_wrapper<T>, sizeof...(Args) >{{inArgs...}};
    }
}
