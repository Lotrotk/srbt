#pragma once

#include "Compiler/Properties.hpp"

namespace SRBT
{
namespace Interpret
{
	bool isBool(Compiler::CompleteType const &ct);
	bool isInteger(Compiler::CompleteType const &ct);
	bool isReal(Compiler::CompleteType const &ct);
	bool isWord(Compiler::CompleteType const &ct);
	bool isType(Compiler::CompleteType const &ct);
	/**
	 * @return kNo_custom_type on failure
	 */
	Compiler::PrimitiveType tryCustomTypeOfObject(Compiler::CompleteType const &ct);
}
}
