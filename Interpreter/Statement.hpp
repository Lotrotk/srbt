#pragma once

#include "Exception.hpp"
#include "IdentifyType.hpp"
#include "Keys.hpp"
#include "Store.hpp"

#include "Compiler/Module.hpp"

#include "Tokenize/Tree.hpp"

namespace SRBT
{
namespace Interpret
{
	bool tryInterpretMember(Store &store, Tokenize::SequenceNode&, Tokenize::iterator_t &);
	Compiler::PropertyPtr interpretValue(Store &store, Tokenize::SequenceNode&, Tokenize::iterator_t &);
}
}
