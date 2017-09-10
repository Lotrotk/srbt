#include "Statement.hpp"

#include "FirstRound/Regex.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;

void SRBT::Interpret::interpretDeclaraion(Store &store, Tokenize::iterator_t const begin, Tokenize::iterator_t const end)
{
	Tokenize::iterator_t it = begin;
	Tokenize::TreeNode *head = &**it;

	FR::CompleteTypePtr completeType;
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
			Store::Value const * const v = store.find(customType->token());
			if(!v)
			{
				throw ParseException(store.origin().fileOrigin().path(), head->line(), customType->token() + " was not defined");
			}
			FR::TProperty *const t = dynamic_cast<FR::TProperty*>(v->_property.get());
			if(!t)
			{
				throw ParseException(store.origin().fileOrigin().path(), head->line(), customType->token() + " is not a type");
			}

			if(FR::TCProperty const*const ct = t->tryAsConstant())
			{
				completeType = ct->value();
			}
			else
			{
				//unknown
				completeType = FR::TProperty::type;
			}
		}
		else
		{
			switch(type->key())
			{
			case Keywords::kBool:
				completeType = FR::BProperty::type;
				break;
			case Keywords::kInteger:
				completeType = FR::IProperty::type;
				break;
			case Keywords::kReal:
				completeType = FR::RProperty::type;
				break;
			case Keywords::kString:
				completeType = FR::SProperty::type;
				break;
			case Keywords::kType:
				completeType = FR::TProperty::type;
				break;
			default:
				throw ParseException(store.origin().fileOrigin().path(), head->line(), "invalid keyword");
			}
		}
	}

	++it;
	if(it == end)
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "variable is missing name");
	}
	head = &**it;

	Tokenize::TokenNode const *const name = head->tryAsToken();
	if(!name || FR::validPropertyName(name->token()))
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "expecting valid variable name");
	}

	if(store.find(name->token()))
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "that name has already been taken");
	}

	++it;
	if(it == end)
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
	if(it == end)
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "variable is missing definition");
	}
	head = &**it;

	FR::PropertyPtr const value = interpretExpression(it, end);
	if(!value)
	{
		throw ParseException(store.origin().fileOrigin().path(), head->line(), "invalid definition");
	}

	store.insert_unique(name->token(), value, completeType);
}
