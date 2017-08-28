#include "ReferencedProperties.hpp"

#include "Module.hpp"

using namespace SRBT;
using namespace SRBT::Compiler;

namespace
{
	class MyAccessor : public MAccess
	{
	public:
		static PropertyPtr find(Module const & module, std::string const & name)
		{
			Module::Properties::const_iterator const it = MAccess::const_ioProperties(module).find(name);
			return it == MAccess::const_ioProperties(module).cend() ? PropertyPtr() : it->second.property();
		}
	};
}

namespace SRBT
{
namespace Compiler
{
	template<> class ReferencedProperty<BTypeDef>;
	template<> class ReferencedProperty<ITypeDef>;
	template<> class ReferencedProperty<RTypeDef>;
	template<> class ReferencedProperty<WTypeDef>;
	template<> class ReferencedProperty<OTypeDef>;
}
}
