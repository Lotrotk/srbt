#include "PropertyNames.hpp"

namespace SRBT
{
namespace Compiler
{

template<>
std::string Name<Compiler::BProperty>()
{
	return "bool";
}

template<>
std::string Name<Compiler::IProperty>()
{
	return "integer";
}

template<>
std::string Name<Compiler::RProperty>()
{
	return "real";
}

template<>
std::string Name<Compiler::WProperty>()
{
	return "word";
}

template<>
std::string Name<Compiler::OProperty>()
{
	return "object";
}

template<>
std::string Name<Compiler::StructProperty>()
{
	return "struct";
}

}
}
