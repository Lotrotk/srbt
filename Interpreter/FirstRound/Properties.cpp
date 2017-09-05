#include "Properties.hpp"

#include "Module.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;
using namespace SRBT::Interpret::FR;

namespace SRBT
{
namespace Interpret
{
namespace FR
{
	CompleteType ObjectProperty::getType() const
	{
		return CompleteType{PrimitiveType::kObject, PrimitiveType(_module.moduleId())};
	}

	bool ObjectProperty::isType(CompleteType const &t) const
	{
		return t.size() == 2 && t.front() == PrimitiveType::kObject && t.back() == PrimitiveType(_module.moduleId());
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CompleteType TypeProperty::getType() const
	{
		CompleteType res;
		res.push_back(PrimitiveType::kType);
		res.insert(res.end(), _myType.cbegin(), _myType.cend());
		return res;
	}

	bool TypeProperty::isType(CompleteType const &t) const
	{
		return t.front() == PrimitiveType::kType && std::equal(std::next(_myType.cbegin()), _myType.cend(), t.cbegin(), t.cend());
	}
}
}
}
