#include "IdentifyType.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;

namespace SRBT
{
namespace Interpret
{
	bool isBool(Compiler::CompleteType const &ct) { return ct.size() == 1 && ct.front() == Compiler::PrimitiveType::kBool; }
	bool isInteger(Compiler::CompleteType const &ct) { return ct.size() == 1 && ct.front() == Compiler::PrimitiveType::kInteger; }
	bool isReal(Compiler::CompleteType const &ct) { return ct.size() == 1 && ct.front() == Compiler::PrimitiveType::kReal; }
	bool isString(Compiler::CompleteType const &ct) { return ct.size() == 1 && ct.front() == Compiler::PrimitiveType::kString; }
	bool isType(Compiler::CompleteType const &ct) { return ct.size() == 1 && ct.front() == Compiler::PrimitiveType::kType; }

	Compiler::PrimitiveType tryCustomTypeOfObject(Compiler::CompleteType const &ct)
	{
		if(ct.size() == 1 && ct.front() > Compiler::PrimitiveType::kNo_custom_type)
		{
			return ct.front();
		}

		return Compiler::PrimitiveType::kNo_custom_type;
	}
}
}
