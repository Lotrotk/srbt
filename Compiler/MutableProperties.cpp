#include "MutableProperties.hpp"

namespace SRBT
{
namespace Compiler
{
	template<> class MutableProperty<BTypeDef>;
	template<> class MutableProperty<ITypeDef>;
	template<> class MutableProperty<RTypeDef>;
	template<> class MutableProperty<WTypeDef>;
	template<> class MutableProperty<OTypeDef>;
}
}
