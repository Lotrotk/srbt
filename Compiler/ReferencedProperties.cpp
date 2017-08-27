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
	bool ReferencedPropertyData::dependend_on_property(Module const & module, std::string const & name) const
	{
		/*first check self*/
		{
			Object const & object = _object->value();
			Module const & myModule = object.module();
			PropertyPtr const p = MyAccessor::find(myModule, _member);
			if(&myModule == &module && _member == name)
			{
				//if overwritten, then no problem
				return object.mapping().find(p) == object.mapping().cend();
			}
			if(p && p->dependend_on_property(module, name))
			{
				return true;
			}
		}
		/*check object*/
		return _object->dependend_on_property(module, name);
	}

	template<> class ReferencedProperty<BTypeDef>;
	template<> class ReferencedProperty<ITypeDef>;
	template<> class ReferencedProperty<RTypeDef>;
	template<> class ReferencedProperty<WTypeDef>;
	template<> class ReferencedProperty<OTypeDef>;
}
}
