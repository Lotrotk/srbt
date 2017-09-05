#include "IdentifyType.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;

namespace SRBT
{
namespace Interpret
{
	bool isBool(FR::CompleteType const &ct) { return ct.size() == 1 && ct.front() == FR::PrimitiveType::kBool; }
	bool isInteger(FR::CompleteType const &ct) { return ct.size() == 1 && ct.front() == FR::PrimitiveType::kInteger; }
	bool isReal(FR::CompleteType const &ct) { return ct.size() == 1 && ct.front() == FR::PrimitiveType::kReal; }
	bool isString(FR::CompleteType const &ct) { return ct.size() == 1 && ct.front() == FR::PrimitiveType::kString; }
	bool isType(FR::CompleteType const &ct) { return ct.size() == 1 && ct.front() == FR::PrimitiveType::kType; }

	FR::PrimitiveType tryCustomTypeOfObject(FR::CompleteType const &ct)
	{
		if(ct.size() == 1 && ct.front() > FR::PrimitiveType::kNo_custom_type)
		{
			return ct.front();
		}

		return FR::PrimitiveType::kNo_custom_type;
	}
}
}
