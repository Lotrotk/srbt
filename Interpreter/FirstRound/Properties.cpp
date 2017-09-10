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
	bool BasicCompleteType::operator==(BasicCompleteType const &t) const
	{
		return t._primitive == _primitive;
	}

	bool BasicCompleteType::operator==(CompleteType const &t) const
	{
		BasicCompleteType const*const b = dynamic_cast<BasicCompleteType const*>(&t);
		return b && (*b == *this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<> std::shared_ptr<BasicCompleteType> const BasicProperty<PrimitiveType::kBool, BCProperty>::type{new BasicCompleteType(PrimitiveType::kBool)};
	template<> std::shared_ptr<BasicCompleteType> const BasicProperty<PrimitiveType::kInteger, ICProperty>::type{new BasicCompleteType(PrimitiveType::kInteger)};
	template<> std::shared_ptr<BasicCompleteType> const BasicProperty<PrimitiveType::kReal, RCProperty>::type{new BasicCompleteType(PrimitiveType::kReal)};
	template<> std::shared_ptr<BasicCompleteType> const BasicProperty<PrimitiveType::kString, SCProperty>::type{new BasicCompleteType(PrimitiveType::kString)};
	template<> std::shared_ptr<BasicCompleteType> const BasicProperty<PrimitiveType::kType, TCProperty>::type{new BasicCompleteType(PrimitiveType::kType)};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void ReferencedProperty::merge(ReferencedProperty &&continuation)
	{
		continuation._ref_name.splice(_ref_name.end(), _ref_name, continuation._ref_name.begin(), continuation._ref_name.end());
	}
}
}
}
