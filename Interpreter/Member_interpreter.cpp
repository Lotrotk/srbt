#include "Statement.hpp"

#include "Compiler/Conversion.hpp"
#include "Compiler/Regex.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;

bool SRBT::Interpret::tryInterpretMember(Store &store, Tokenize::SequenceNode &sequence, Tokenize::iterator_t &it)
{
	Tokenize::TreeNode *head = &**it;

	Compiler::CompleteType completeType;
	{
		Tokenize::KeyNode const *const type = head->tryAsKeyNode();
		if(!type)
		{
			/*perhaps it is a custom type ?*/
			Tokenize::TokenNode const *customType = head->tryAsToken();
			if(!customType)
			{
				throw ParseException(store.origin().fileOrigin().path(), head->line(), "member is missing type");
			}
			Compiler::PropertyPtr const * const p = store.find(customType->token());
			if(!p)
			{
				throw ParseException(store.origin().fileOrigin().path(), head->line(), customType->token() + " was not defined");
			}
			if(!isType(p->get()->getType()))
			{
				throw ParseException(store.origin().fileOrigin().path(), head->line(), customType->token() + " is not a type");
			}

			Compiler::TypeProperty const &tproperty = static_cast<Compiler::TypeProperty const&>(**p);
			completeType = tproperty.getType();
		}
		else
		{
			switch(type->key())
			{
			case Keywords::kBool:
				completeType = Compiler::BProperty::type();
				break;
			case Keywords::kInteger:
				completeType = Compiler::IProperty::type();
				break;
			case Keywords::kReal:
				completeType = Compiler::RProperty::type();
				break;
			case Keywords::kString:
				completeType = Compiler::SProperty::type();
				break;
			case Keywords::kType:
				completeType = Compiler::CompleteType{Compiler::PrimitiveType::kType};
				break;
			default:
				return false;
			}
		}
	}

	++it;
	if(it == sequence.list().end())
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "variable is missing name");
	}
	head = &**it;

	Tokenize::TokenNode const *const name = head->tryAsToken();
	if(!name || Compiler::validPropertyName(name->token()))
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "expecting valid variable name");
	}

	if(store.find(name->token()))
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "that name has already been taken");
	}

	++it;
	if(it == sequence.list().end())
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "variable is missing definition");
	}
	head = &**it;

	Tokenize::KeyNode const *const defoperator = head->tryAsKeyNode();
	if(!defoperator || defoperator->key() != Operators::kIs)
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "expected '=' after variable name");
	}

	++it;
	if(it == sequence.list().end())
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "variable is missing definition");
	}
	head = &**it;

	Compiler::PropertyPtr const value = Compiler::Conversion::tryConvertTo(interpretValue(store, sequence, it), completeType);
	if(!value)
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "invalid definition");
	}

	store.insert_unique(name->token(), value);

	return true;
}
