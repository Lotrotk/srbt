#include "Statement.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;

namespace
{
	Compiler::PropertyPtr interpretSingleValue(Store &store, Tokenize::TreeNode &node)
	{
		Compiler::PropertyPtr res;

		if(Tokenize::StringNode *const s = node.tryAsString())
		{
			res.reset(new Compiler::SCProperty(s->value()));
		}
		//else if(Tokenize::TokenNode *const token = node.tryAsToken())
		{

		}
		//else if(Tokenize::KeyNode *const key = node.tryAsKeyNode())
		{

		}
		//else if(Tokenize::BracesNode *const bracesNode = node.tryAsBracesNode())
		{

		}

		return res;
	}
}

Compiler::PropertyPtr SRBT::Interpret::interpretValue(Store &store, Tokenize::SequenceNode &sequence, Tokenize::iterator_t &it)
{
	Compiler::PropertyPtr res;

	Tokenize::TreeNode *head = &**it;
	if(interpretSingleValue(store, *head))
	{

	}

	return res;
}
