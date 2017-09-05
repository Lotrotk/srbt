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

int_t toInteger(std::string const & word)
{
	int_t value;
	std::stringstream(word) >> value;
	return value;
}

real_t toReal(std::string const & word)
{
	real_t value;
	std::stringstream(word) >> value;
	return value;
}

}
}
}
