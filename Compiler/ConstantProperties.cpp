#include "ConstantProperties.hpp"

namespace SRBT
{
namespace Compiler
{
	template<> class ConstantCachedProperty<BTypeDef>;
	template<> class ConstantCachedProperty<ITypeDef>;
	template<> class ConstantCachedProperty<RTypeDef>;
	template<> class ConstantCachedProperty<WTypeDef>;
	template<> class ConstantProperty<OTypeDef>;

	template<> BCProperty::Cache BCProperty::_cache{};
	template<> ICProperty::Cache ICProperty::_cache{};
	template<> RCProperty::Cache RCProperty::_cache{};
	template<> WCProperty::Cache WCProperty::_cache{};
}
}
