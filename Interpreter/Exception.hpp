#pragma once

#include "Tokenize/File.hpp"

#include <ostream>

namespace SRBT
{
namespace Interpret
{
	class ParseException
	{
	public:
		ParseException(Tokenize::path_t const &filename, const int line, std::string &&what) : _filename(filename), _what(std::move(what)), _line(line) {}

	private:
		Tokenize::path_t _filename;
		std::string _what;
		int _line;

	private:
		friend std::ostream &operator<<(std::ostream &os, ParseException const &e)
		{
			return os << e._filename << ':' << e._line << ": " << e._what;
		}
	};
}
}
