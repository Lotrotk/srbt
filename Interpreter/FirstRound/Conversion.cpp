#include "Conversion.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;
using namespace SRBT::Interpret::FR;

PropertyPtr SRBT::Interpret::FR::Conversion::tryConvertTo(PropertyPtr const &property, CompleteType const &type)
{
	PropertyPtr res;

	if(property->isType(type))
	{
		res = property;
	}
	else if(type == BProperty::type())
	{
		if(IPropertyPtr delegate = std::dynamic_pointer_cast<IProperty>(property))
		{
			res.reset(new Conversion::IntToBool(delegate));
		}
		else if(RPropertyPtr delegate = std::dynamic_pointer_cast<RProperty>(property))
		{
			res.reset(new Conversion::RealToBool(delegate));
		}
	}
	else if(type == IProperty::type())
	{
		if(BPropertyPtr delegate = std::dynamic_pointer_cast<BProperty>(property))
		{
			res.reset(new Conversion::BoolToInt(delegate));
		}
		else if(RPropertyPtr delegate = std::dynamic_pointer_cast<RProperty>(property))
		{
			res.reset(new Conversion::RealToInt(delegate));
		}
	}
	else if(type == RProperty::type())
	{
		if(BPropertyPtr delegate = std::dynamic_pointer_cast<BProperty>(property))
		{
			res.reset(new Conversion::BoolToReal(delegate));
		}
		else if(IPropertyPtr delegate = std::dynamic_pointer_cast<IProperty>(property))
		{
			res.reset(new Conversion::IntToReal(delegate));
		}
	}
	else if(type == SProperty::type())
	{
		if(BPropertyPtr delegate = std::dynamic_pointer_cast<BProperty>(property))
		{
			res.reset(new Conversion::BoolToString(delegate));
		}
		else if(IPropertyPtr delegate = std::dynamic_pointer_cast<IProperty>(property))
		{
			res.reset(new Conversion::IntToString(delegate));
		}
		else if(RPropertyPtr delegate = std::dynamic_pointer_cast<RProperty>(property))
		{
			res.reset(new Conversion::RealToString(delegate));
		}
	}
	else if(type.size() == 1 && type.front() == PrimitiveType::kType)
	{
		if(dynamic_cast<TypeProperty*>(property.get()))
		{
			res = property;
		}
	}

	return res;
}
