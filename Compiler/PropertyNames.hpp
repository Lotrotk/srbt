#pragma once

#include "ConstantProperties.hpp"

namespace SRBT
{
namespace Compiler
{
	template<typename property>
	std::string Name();

	template<>
	std::string Name<Compiler::BProperty>();

	template<>
	std::string Name<Compiler::IProperty>();

	template<>
	std::string Name<Compiler::RProperty>();

	template<>
	std::string Name<Compiler::WProperty>();

	template<>
	std::string Name<Compiler::OProperty>();

	template<>
	std::string Name<Compiler::StructProperty>();
}
}
