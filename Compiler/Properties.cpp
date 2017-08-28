#include "Properties.hpp"

#include "Module.hpp"

using namespace SRBT;
using namespace SRBT::Compiler;

namespace SRBT
{
namespace Compiler
{
namespace prvt
{
	CompleteType &object_completetype_return::ret(Object const &o)
	{
		return o.module().objectType();
	}
}
}
}
