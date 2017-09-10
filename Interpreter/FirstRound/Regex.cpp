#include "Regex.hpp"

#include <regex>
#include <sstream>

using namespace SRBT;
using namespace SRBT::Interpret;
using namespace SRBT::Interpret::FR;

namespace SRBT
{
namespace Interpret
{
namespace SRBT
{

bool validPropertyName(std::string const & word)
{
	std::regex regex("^[a-zA-Z_][0-9a-zA-Z_]*$");
	std::smatch match;
	return std::regex_match(word, match, regex);
}

bool validInteger(std::string const & word, int_t &value)
{
	std::regex regex("^[0-9]+$");
	std::smatch match;
	if(std::regex_match(word, match, regex))
	{
		std::stringstream(word) >> value;
		return true;
	}
	return false;
}

bool validReal(std::string const & word, real_t &value)
{
	std::regex regex("^[0-9]+[.][0-9]*$");
	std::smatch match;
	if(std::regex_match(word, match, regex))
	{
		std::stringstream(word) >> value;
		return true;
	}
	return false;
}

bool validBool(std::string const &word, bool &value)
{
	if(word == "true")
	{
		value = true;
		return true;
	}
	if(word == "false")
	{
		value = false;
		return true;
	}
	return false;
}

}
}
}
