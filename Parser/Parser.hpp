#pragma once

#include "ParseLib/File.hpp"
#include "Compiler/Module.hpp"


namespace SRBT
{
namespace Parser
{
	/**
	 * @throws : ParseException
	 */
	Compiler::ModulePtr readModule(Parse::File const&);
}
}
