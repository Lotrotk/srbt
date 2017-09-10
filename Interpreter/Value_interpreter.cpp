#include "Statement.hpp"

#include "FirstRound/Regex.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;

namespace
{
	struct Argument final
	{
		Store &_store;
		Tokenize::SequenceNode &_sequence;
	};

	/**
	 * @brief Reads a reference further on the same line (e.g. a. b.c.  d.e)
	 * @param begin : points to the first reference. If this functions extends it, it will be set to point to the last reference
	 * @param reference : the first reference, at begin
	 */
	void interpretReference(Tokenize::SequenceNode &, Tokenize::iterator_t &begin, FR::ReferencedProperty &reference);
	/**
	 * @brief reads a single token, or multiple if they form a group (parentheses, reference)
	 * @param begin : points to the current item, is set to point to the last item read
	 */
	FR::PropertyPtr interpretSingleValue(Tokenize::SequenceNode &, Tokenize::iterator_t &begin);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void interpretReference(Tokenize::SequenceNode &sequence, Tokenize::iterator_t &begin, FR::ReferencedProperty &reference)
	{
		int const line = static_cast<Tokenize::TreeNode&>(**begin).line();
		Tokenize::iterator_t it = std::next(begin);
		if(it == sequence.list().end() || static_cast<Tokenize::TreeNode&>(**it).line() != line)
		{
			return;
		}
		Tokenize::TreeNode &point = **it;
		Tokenize::KeyNode *const keyNode = point.tryAsKeyNode();
		if(!keyNode || keyNode->key() != Operators::kPoint)
		{
			return;
		}
		++it;
		if(it == sequence.list().end() || static_cast<Tokenize::TreeNode&>(**it).line() != line)
		{
			return;
		}
		FR::PropertyPtr const continued = interpretSingleValue(sequence, it);
		if(std::shared_ptr<FR::ReferencedProperty> r = std::dynamic_pointer_cast<FR::ReferencedProperty>(continued))
		{
			reference.merge(std::move(*r));
			begin = it;
		}
	}

	FR::PropertyPtr interpretSingleValue(Tokenize::SequenceNode &sequence, Tokenize::iterator_t &begin)
	{
		Tokenize::TreeNode &node = **begin;

		FR::PropertyPtr res;

		if(Tokenize::StringNode *const s = node.tryAsString())
		{
			res.reset(new FR::SCProperty(s->value()));
		}
		else if(Tokenize::TokenNode *const token = node.tryAsToken())
		{
			FR::int_t value_int;
			FR::real_t value_real;
			bool value_bool;
			if(FR::validInteger(token->token(), value_int))
			{
				res.reset(new FR::ICProperty(value_int));
			}
			else if(FR::validReal(token->token(), value_real))
			{
				res.reset(new FR::RCProperty(value_real));
			}
			else if(FR::validBool(token->token(), value_bool))
			{
				res.reset(new FR::BCProperty(value_bool));
			}
			else if(FR::validPropertyName(token->token()))
			{
				std::shared_ptr<FR::ReferencedProperty> reference(new FR::ReferencedProperty(std::string(token->token())));
				interpretReference(sequence, begin, *reference);
				res = reference;
			}
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

FR::PropertyPtr SRBT::Interpret::interpretValue(Tokenize::SequenceNode &sequence, Tokenize::iterator_t &it)
{
	return ::interpretSingleValue(sequence, it);
}
