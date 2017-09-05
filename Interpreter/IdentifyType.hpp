#pragma once

#include "FirstRound/Properties.hpp"

namespace SRBT
{
namespace Interpret
{
	bool isBool(FR::CompleteType const &ct);
	bool isInteger(FR::CompleteType const &ct);
	bool isReal(FR::CompleteType const &ct);
	bool isWord(FR::CompleteType const &ct);
	bool isType(FR::CompleteType const &ct);
	/**
	 * @return kNo_custom_type on failure
	 */
	FR::PrimitiveType tryCustomTypeOfObject(FR::CompleteType const &);
}
}
