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
	bool tryInterpretMember(Store &store, Tokenize::SequenceNode&, Tokenize::iterator_t &);
	FR::PropertyPtr interpretValue(Tokenize::SequenceNode&, Tokenize::iterator_t &);
}
}
