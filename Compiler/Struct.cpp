#include "Struct.hpp"

namespace SRBT
{
namespace Compiler
{
	template<> class ConstantCachedProperty<STypeDef>;
	template<> StructCProperty::Cache StructCProperty::_cache{};
}
}
