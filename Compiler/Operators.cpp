#include "Operators.hpp"

namespace SRBT
{
namespace Compiler
{
namespace Operators
{
namespace prvt
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// unary operators -- further implementation //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BPropertyPtr prvt::Negate<BTypeDef, BTypeDef>::reduced(BPropertyPtr const & arg)
	{
		if(NotBProperty const * const p = dynamic_cast<NotBProperty const *>(arg.get()))
		{
			return p->_arg;
		}

		return BPropertyPtr();
	}

	IPropertyPtr prvt::Negate<ITypeDef, ITypeDef>::reduced(IPropertyPtr const & arg)
	{
		if(MinusIProperty const * const p = dynamic_cast<MinusIProperty const *>(arg.get()))
		{
			return p->_arg;
		}

		return IPropertyPtr();
	}

	RPropertyPtr prvt::Negate<RTypeDef, RTypeDef>::reduced(RPropertyPtr const & arg)
	{
		if(MinusRProperty const * const p = dynamic_cast<MinusRProperty const *>(arg.get()))
		{
			return p->_arg;
		}

		return RPropertyPtr();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<> class BinaryOperator<BTypeDef, BTypeDef, prvt::Arithmetic>;
	template<> class BinaryOperator<ITypeDef, ITypeDef, prvt::Arithmetic>;
	template<> class BinaryOperator<RTypeDef, RTypeDef, prvt::Arithmetic>;

	template<> class BinaryOperator<BTypeDef, ITypeDef, prvt::Compare>;
	template<> class BinaryOperator<BTypeDef, RTypeDef, prvt::Compare>;
	template<> class BinaryOperator<BTypeDef, WTypeDef, prvt::Compare>;
}
}
}
}
