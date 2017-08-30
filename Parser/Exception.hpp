#pragma once

#include "ParseLib/File.hpp"

namespace SRBT
{
namespace Parser
{
	class ParseException
	{
	public:
		ParseException(Parse::path_t const &filename, const int line, std::string &&what) : _filename(filename), _what(std::move(what)), _line(line) {}

	private:
		Parse::path_t _filename;
		std::string _what;
		int _line;
	};
}
}
