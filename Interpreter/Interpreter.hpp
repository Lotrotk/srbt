#pragma once

#include "Tokenize/File.hpp"
#include "Compiler/Module.hpp"


namespace SRBT
{
namespace Interpret
{
	/**
	 * @throws : ParseException
	 */
	Compiler::ModulePtr interpretModule(Tokenize::File const&);
}
}
