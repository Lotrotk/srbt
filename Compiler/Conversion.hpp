#pragma once

#include "Module.hpp"
#include "Operators_Unary.hpp"

namespace SRBT
{
namespace Compiler
{
namespace Conversion
{
	class ModuleReader
	{
	public:
		virtual ~ModuleReader() = default;
		virtual Module const & read(std::string const &) const = 0;
	};

	StructPropertyPtr tryToStruct(PropertyPtr const & property, ModuleReader const &, Compiler::FileOrigin const &, std::string & errorMessage);
	OPropertyPtr tryToObject(PropertyPtr const & property, std::string & errorMessage);
	BPropertyPtr tryToBool(PropertyPtr const & property, std::string & errorMessage);
	IPropertyPtr tryToInteger(PropertyPtr const & property, std::string & errorMessage);
	RPropertyPtr tryToReal(PropertyPtr const & property, std::string & errorMessage);
	WPropertyPtr tryToWord(PropertyPtr const & property, std::string & errorMessage);

	PropertyPtr tryTo(PrimitiveType type, PropertyPtr const & property, ModuleReader const & reader, Compiler::FileOrigin const & origin, std::string & errorMessage);
}
}
}
