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
	bool validInteger(std::string const &, int_t &outValue);
	bool validReal(std::string const &, real_t &outValue);
	bool validBool(std::string const &, bool &outValue);
}
}
}
