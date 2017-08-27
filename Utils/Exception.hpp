#pragma once

#include <iostream>

namespace Utils
{
	class Unexpected
	{
	public:
		Unexpected(char const * const inFile, int const inLine, char const * const inMessage) : _line(inLine), _file(inFile), _msg(inMessage) {}

	private:
		int _line;
		char const * _file;
		char const * _msg;

	public:
		friend std::ostream & operator<<(std::ostream & os, Unexpected const & exc)
		{
			return os << exc._file << ':' << exc._line << " : " << exc._msg;
		}
	};

	class TechnicalException
	{
	public:
		TechnicalException(std::string const & what) : _what(what) {}

	private:
		std::string _what;

	public:
		friend std::ostream & operator<<(std::ostream & os, TechnicalException const & exc)
		{
			return os << "Error : " << exc._what;
		}
	};
}
