#include "Statement.hpp"

#include "FirstRound/Operators.hpp"
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
	 * @param begin : points to lastest element already read in sequence, set to last element in sequence (unchanged if no sequence)
	 */
	void interpretReference(Tokenize::iterator_t &begin, Tokenize::iterator_t const end, FR::ReferencedProperty &reference);
	/**
	 * @brief reads a single token, or multiple if they form a group (parentheses, reference)
	 * @param begin : set to last element in sequence (unchanged if no sequence)
	 */
	FR::PropertyPtr interpretSingleValue(Tokenize::path_t const &path, Tokenize::iterator_t &begin, Tokenize::iterator_t end);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void interpretReference(Tokenize::iterator_t &begin, Tokenize::iterator_t const end, FR::ReferencedProperty &reference)
	{
		while(true)
		{
			Tokenize::iterator_t it = std::next(begin);
			if(it == end)
			{
				return;
			}
			Tokenize::KeyNode *const keyNode = static_cast<Tokenize::TreeNode&>(**it).tryAsKeyNode();
			if(!keyNode || keyNode->key() != Operators::kPoint)
			{
				return;
			}
			++it;
			if(it == end)
			{
				return;
			}

			if(Tokenize::TokenNode *const token = static_cast<Tokenize::TreeNode&>(**it).tryAsToken())
			{
				FR::int_t value_int;
				FR::real_t value_real;
				bool value_bool;
				if(FR::validInteger(token->token(), value_int) || FR::validReal(token->token(), value_real) || FR::validBool(token->token(), value_bool) || !FR::validPropertyName(token->token()))
				{
					reference.append(std::string(token->token()));
				}
				begin = it;
			}
		}
	}

	FR::PropertyPtr interpretSingleValue(Tokenize::path_t const &path, Tokenize::iterator_t &begin, Tokenize::iterator_t const end)
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
				interpretReference(begin, end, *reference);
				res = reference;
			}
		}
		else if(Tokenize::KeyNode *const key = node.tryAsKeyNode())
		{
			using Operand = FR::Operators::UnaryOperator::Operand;
			Operand operand;
			switch (key->key())
			{
			case Operators::kMinus:
				operand = Operand::kMinus;
				break;
			case Operators::kExclemation:
				operand = Operand::kNot;
			default:
				throw ParseException(path, key->line(), "Expected expression");
			}
			++begin;
			res.reset(new FR::Operators::UnaryOperator(operand, interpretValue(path, begin, end)));
		}
		else if(Tokenize::BracesNode *const bracesNode = node.tryAsBracesNode())
		{
			if(bracesNode->leftBrace() == Operators::kBraceLeft)
			{
				res = interpretExpression(bracesNode->list().begin(), bracesNode->list().end());
			}
		}

		if(!res)
		{
			throw ParseException(path, node.line(), "Unable to read value");
		}

		return res;
	}
}

FR::PropertyPtr SRBT::Interpret::interpretValue(Tokenize::path_t const &path, Tokenize::iterator_t &begin, Tokenize::iterator_t const end)
{
	return ::interpretSingleValue(path, begin, end);
}
