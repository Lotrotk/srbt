#pragma once

#include <string>

#include "Properties.hpp"

namespace SRBT
{
namespace Interpret
{
namespace FR
{
	bool validPropertyName(std::string const &);
	bool validReferencedName(std::string const &);
	bool validInteger(std::string const &);
	bool validReal(std::string const &);

	int_t toInteger(std::string const &);
	real_t toReal(std::string const &);
}
}
}
