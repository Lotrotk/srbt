#pragma once

#include "Tokenize/File.hpp"
#include "FirstRound/Module.hpp"


namespace SRBT
{
namespace Interpret
{
	/**
	 * @throws : ParseException
	 */
	FR::ModulePtr interpretModule(Tokenize::File const&);
}
}
