#include "Statement.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;

void SRBT::Interpret::interpretMember(Compiler::Module &module, Tokenize::SequenceNode &sequence, Tokenize::iterator_t &it)
{
	Tokenize::TreeNode &head = **it;
	Tokenize::KeyNode const *const type = head.tryAsKeyNode();
	Tokenize::TokenNode const * customType{};
	if(!type)
	{
		customType = head.tryAsToken();
		if(!customType)
		{
			throw ParseException(module.origin().fileOrigin().path(), head.line(), "member is missing type");
		}
	}

	switch(type->key())
	{
	case Keywords::kBool:
	case Keywords::kInteger:
	case Keywords::kReal:
	case Keywords::kWord:
	case Keywords::kStruct:
	default:
		break;
	}
}
