#pragma once

#include "Exception.hpp"
#include "Keys.hpp"
#include "Store.hpp"

#include "FirstRound/Module.hpp"

#include "Tokenize/Tree.hpp"

namespace SRBT
{
namespace Interpret
{
	/**
	 * @brief reads a declaration
	 * @param store : where to store the declaration + definition
	 */
	void interpretDeclaraion(Store &store, Tokenize::iterator_t begin, Tokenize::iterator_t end);
	/**
	 * @brief reads an expression
	 */
	FR::PropertyPtr interpretExpression(Tokenize::iterator_t begin, Tokenize::iterator_t end);
	/**
	 * @brief reads a single value
	 * @param begin : set to last element read
	 */
	FR::PropertyPtr interpretValue(Tokenize::path_t const &path, Tokenize::iterator_t &begin, Tokenize::iterator_t end);
}
}
