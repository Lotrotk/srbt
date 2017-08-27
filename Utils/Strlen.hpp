#pragma once

namespace Utils
{
	constexpr size_t strlen(char const * const s)
	{
		return s[0] == 0 ? 0 : 1 + strlen(s + 1);
	}
}
