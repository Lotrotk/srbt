#include "Regex.hpp"

#include <regex>
#include <sstream>

namespace SRBT
{
namespace Compiler
{

bool validPropertyName(std::string const & word)
{
	std::regex regex("^[a-zA-Z_][0-9a-zA-Z_]*$");
	std::smatch match;
	return std::regex_match(word, match, regex);
}

bool validReferencedName(std::string const & word)
{
	std::regex regex("^[a-zA-Z_][0-9a-zA-Z_]*([.][a-zA-Z_][0-9a-zA-Z_]*)*$");
	std::smatch match;
	return std::regex_match(word, match, regex);
}

bool validInteger(std::string const & word)
{
	std::regex regex("^[0-9]+$");
	std::smatch match;
	return std::regex_match(word, match, regex);
}

bool validReal(std::string const & word)
{
	std::regex regex("^[0-9]+[.][0-9]*$");
	std::smatch match;
	return std::regex_match(word, match, regex);
}

IProperty::value_t toInteger(std::string const & word)
{
	IProperty::value_t value;
	std::stringstream(word) >> value;
	return value;
}

RProperty::value_t toReal(std::string const & word)
{
	RProperty::value_t value;
	std::stringstream(word) >> value;
	return value;
}

}
}
