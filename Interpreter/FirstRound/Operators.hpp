#pragma once

#include "Properties.hpp"

namespace SRBT
{
namespace Interpret
{
namespace FR
{
namespace Operators
{
	class UnaryOperator final : public Property
	{
	public:
		enum struct Operand{ kMinus, kNot };

	public:
		UnaryOperator(Operand const op, PropertyPtr const &delegate) : _op(op), _delegate(delegate) {}

		Operand operand() const { return _op; }
		PropertyPtr const &delegate() const { return _delegate; }

	private:
		Operand const _op;
		PropertyPtr const _delegate;
	};
}
}
}
}
