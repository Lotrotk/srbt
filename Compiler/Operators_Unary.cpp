#include "Operators_Unary.hpp"

namespace SRBT
{
namespace Compiler
{
namespace Operators
{
	template<> class UnaryOperator<BTypeDef, BTypeDef, prvt::Negate>;
	template<> class UnaryOperator<ITypeDef, ITypeDef, prvt::Negate>;
	template<> class UnaryOperator<RTypeDef, RTypeDef, prvt::Negate>;

	template<> class UnaryOperator<BTypeDef, ITypeDef, prvt::ToBool>;
	template<> class UnaryOperator<BTypeDef, RTypeDef, prvt::ToBool>;

	template<> class UnaryOperator<ITypeDef, BTypeDef, prvt::ToInteger>;
	template<> class UnaryOperator<ITypeDef, RTypeDef, prvt::ToInteger>;

	template<> class UnaryOperator<RTypeDef, BTypeDef, prvt::ToReal>;
	template<> class UnaryOperator<RTypeDef, ITypeDef, prvt::ToReal>;

	template<> class UnaryOperator<WTypeDef, BTypeDef, prvt::ToWord>;
	template<> class UnaryOperator<WTypeDef, ITypeDef, prvt::ToWord>;
	template<> class UnaryOperator<WTypeDef, RTypeDef, prvt::ToWord>;
}
}
}
