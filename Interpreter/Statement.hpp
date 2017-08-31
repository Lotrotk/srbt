#pragma once

#include "Exception.hpp"
#include "Keys.hpp"

#include "Compiler/Module.hpp"

#include "Tokenize/Tree.hpp"

namespace SRBT
{
namespace Interpret
{
	void interpretMember(Compiler::Module&, Tokenize::SequenceNode&, Tokenize::iterator_t &);
}
}
