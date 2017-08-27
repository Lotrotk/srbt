#pragma once

#include "ConstantProperties.hpp"

namespace SRBT
{
namespace Compiler
{
	using STypeDef = Typedef<Property::Type::kStruct, Module const &, Module const &, Module const &, prvt::value_return<Module const &, Module const &>>;
	using StructProperty = ValueProperty<STypeDef>;
	MAKE_SHARED_PTR(StructProperty);

	namespace prvt
	{
		template<>
		struct KeyForConstantCachedProperty<STypeDef>
		{
			using key_t = Module const *;
			static inline key_t key(Module const & value)
			{
				return &value;
			}
		};
	}

	using StructCProperty = ConstantCachedProperty<STypeDef>;
}
}
