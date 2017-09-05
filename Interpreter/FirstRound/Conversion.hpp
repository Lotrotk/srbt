#pragma once

#include "Properties.hpp"

namespace SRBT
{
namespace Interpret
{
namespace FR
{
	namespace Conversion
	{
		template<typename in, typename parent>
		class ConversionProperty final : public parent
		{
		public:
			ConversionProperty(in const &delegate) : _delegate(delegate) {}

			in const& delegate() const { return _delegate; }

		private:
			in _delegate;
		};

		using IntToBool = ConversionProperty<IPropertyPtr, BProperty>;
		using RealToBool = ConversionProperty<RPropertyPtr, BProperty>;
		using BoolToInt = ConversionProperty<BPropertyPtr, IProperty>;
		using RealToInt = ConversionProperty<RPropertyPtr, IProperty>;
		using BoolToReal = ConversionProperty<BPropertyPtr, RProperty>;
		using IntToReal = ConversionProperty<IPropertyPtr, RProperty>;
		using BoolToString = ConversionProperty<BPropertyPtr, SProperty>;
		using IntToString = ConversionProperty<IPropertyPtr, SProperty>;
		using RealToString = ConversionProperty<RPropertyPtr, SProperty>;

		PropertyPtr tryConvertTo(PropertyPtr const &, CompleteType const &);
	}
}
}
}
