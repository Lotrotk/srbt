#pragma once

#include "Debug.hpp"
#include "Exception.hpp"

#include <memory>
#include <utility>

#define MAKE_SHARED_PTR(classname) using classname##Ptr = std::shared_ptr<classname>
#define MAKE_SHARED_CONST_PTR(classname) using Const##classname##Ptr = std::shared_ptr<classname const>

namespace Utils
{
	/**
	 *throws: Unexpected if is_target_debug
	 */
	template<typename dest_t, typename in_t>
	inline constexpr dest_t sam_safe_cast(in_t const input, char const * const fileName, int const line)
	{
		if(is_target_debug)
		{
			bool const initialized = input;
			dest_t const res = dynamic_cast<dest_t>(input);
			if(initialized && !res)
			{
				throw Unexpected(fileName, line, "failed to safe cast");
			}

			return res;
		}

		return static_cast<dest_t>(input);
	}

	/**
	 *throws: Unexpected if is_target_debug
	 */
	template<typename dest_t, typename in_t>
	inline constexpr std::shared_ptr<dest_t> sam_safe_shared_cast(in_t && input, char const * const fileName, int const line)
	{
		if(is_target_debug)
		{
			bool const initialized = input.get();
			std::shared_ptr<dest_t> res = std::dynamic_pointer_cast<dest_t>(std::forward<in_t>(input));
			if(initialized && !res)
			{
				throw Unexpected(fileName, line, "failed to safe cast");
			}

			return res;
		}

		return std::static_pointer_cast<dest_t>(std::forward<in_t>(input));
	}
}
