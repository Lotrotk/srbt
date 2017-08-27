#pragma once

#include <string>

#include "Properties.hpp"

namespace SRBT
{
namespace Compiler
{
	bool validPropertyName(std::string const &);
	bool validReferencedName(std::string const &);
	bool validInteger(std::string const &);
	bool validReal(std::string const &);

	IProperty::value_t toInteger(std::string const &);
	RProperty::value_t toReal(std::string const &);
}
}
