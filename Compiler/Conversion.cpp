#include "Conversion.hpp"

#include "PropertyNames.hpp"
#include "Regex.hpp"

namespace SRBT
{
namespace Compiler
{
namespace Conversion
{

StructPropertyPtr tryToStruct(PropertyPtr const & property, ModuleReader const & reader, Compiler::FileOrigin const & origin, std::string & errorMessage)
{
	switch(property->type())
	{
	case Property::Type::kStruct:
		return std::static_pointer_cast<StructCProperty>(property);
	case Property::Type::kWord:
		if(property->constant())
		{
			WProperty::value_t const & word = static_cast<WProperty const &>(*property).value();
			std::experimental::filesystem::path path = origin.path().parent_path();
			path /= word;
			return StructCProperty::instance(reader.read(path));
		}
	default:
		errorMessage = std::string("cannot convert to ") + Name<StructProperty>();
		return StructPropertyPtr();
	}
}

OPropertyPtr tryToObject(PropertyPtr const & property, std::string & errorMessage)
{
	switch(property->type())
	{
	case Property::Type::kObject:
		return std::static_pointer_cast<OProperty>(property);
	case Property::Type::kStruct:
	{
		StructProperty const & s = static_cast<StructProperty const &>(*property);
		return OPropertyPtr(new OCProperty(ObjectPtr(new Object(s.value()))));
	}
	default:
		errorMessage = std::string("cannot convert to ") + Name<OProperty>();
		return OPropertyPtr();
	}
}

BPropertyPtr tryToBool(PropertyPtr const & property, std::string & errorMessage)
{
	switch(property->type())
	{
	case Property::Type::kBool:
		return std::static_pointer_cast<BProperty>(property);
	case Property::Type::kInteger:
		return BPropertyPtr(new Operators::ItoBProperty(std::static_pointer_cast<IProperty>(property)));
	case Property::Type::kReal:
		return BPropertyPtr(new Operators::RtoBProperty(std::static_pointer_cast<RProperty>(property)));
	case Property::Type::kWord:
		if(property->constant())
		{
			WProperty::value_t const & word = static_cast<WProperty const &>(*property).value();
			if(word == "true")
			{
				return BCProperty::instance(true);
			}
			else if(word == "false")
			{
				return BCProperty::instance(false);
			}
		}
	default:
		errorMessage = std::string("cannot convert to ") + Name<BProperty>();
		return BPropertyPtr();
	}
}

IPropertyPtr tryToInteger(PropertyPtr const & property, std::string & errorMessage)
{
	switch(property->type())
	{
	case Property::Type::kInteger:
		return std::static_pointer_cast<IProperty>(property);
	case Property::Type::kBool:
		return IPropertyPtr(new Operators::BtoIProperty(std::static_pointer_cast<BProperty>(property)));
	case Property::Type::kReal:
		return IPropertyPtr(new Operators::RtoIProperty(std::static_pointer_cast<RProperty>(property)));
	case Property::Type::kWord:
		if(property->constant())
		{
			WProperty::value_t const & word = static_cast<WProperty const &>(*property).value();
			if(validInteger(word))
			{
				return ICProperty::instance(toInteger(word));
			}
			if(validReal(word))
			{
				return ICProperty::instance(Operators::prvt::ToReal<RTypeDef, ITypeDef>::operate(toReal(word)));
			}
			if(word == "true")
			{
				return ICProperty::instance(Operators::prvt::ToInteger<ITypeDef, BTypeDef>::operate(true));
			}
			if(word == "false")
			{
				return ICProperty::instance(Operators::prvt::ToInteger<ITypeDef, BTypeDef>::operate(false));
			}
		}
	default:
		errorMessage = std::string("cannot convert to ") + Name<IProperty>();
		return IPropertyPtr();
	}
}

RPropertyPtr tryToReal(PropertyPtr const & property, std::string & errorMessage)
{
	switch(property->type())
	{
	case Property::Type::kReal:
		return std::static_pointer_cast<RProperty>(property);
	case Property::Type::kBool:
		return RPropertyPtr(new Operators::BtoRProperty(std::static_pointer_cast<BProperty>(property)));
	case Property::Type::kInteger:
		return RPropertyPtr(new Operators::ItoRProperty(std::static_pointer_cast<IProperty>(property)));
	case Property::Type::kWord:
		if(property->constant())
		{
			WProperty::value_t const & word = static_cast<WProperty const &>(*property).value();
			if(validReal(word))
			{
				return RCProperty::instance(toReal(word));
			}
			if(validInteger(word))
			{
				return RCProperty::instance(Operators::prvt::ToInteger<ITypeDef, RTypeDef>::operate(toInteger(word)));
			}
			if(word == "true")
			{
				return RCProperty::instance(Operators::prvt::ToReal<RTypeDef, BTypeDef>::operate(true));
			}
			if(word == "false")
			{
				return RCProperty::instance(Operators::prvt::ToReal<RTypeDef, BTypeDef>::operate(false));
			}
		}
	default:
		errorMessage = std::string("cannot convert to ") + Name<RProperty>();
		return RPropertyPtr();
	}
}

WPropertyPtr tryToWord(PropertyPtr const & property, std::string & errorMessage)
{
	switch(property->type())
	{
	case Property::Type::kWord:
		return std::static_pointer_cast<WProperty>(property);
	case Property::Type::kBool:
		return WPropertyPtr(new Operators::BtoWProperty(std::static_pointer_cast<BProperty>(property)));
	case Property::Type::kInteger:
		return WPropertyPtr(new Operators::ItoWProperty(std::static_pointer_cast<IProperty>(property)));
	case Property::Type::kReal:
		return WPropertyPtr(new Operators::RtoWProperty(std::static_pointer_cast<RProperty>(property)));
	default:
		errorMessage = std::string("cannot convert to ") + Name<WProperty>();
		return WPropertyPtr();
	}
}

PropertyPtr tryTo(Property::Type const type, PropertyPtr const & property, ModuleReader const & reader, Compiler::FileOrigin const & origin, std::string & errorMessage)
{
	switch (type)
	{
	case Property::Type::kBool:
		return tryToBool(property, errorMessage);
	case Property::Type::kInteger:
		return tryToInteger(property, errorMessage);
	case Property::Type::kReal:
		return tryToReal(property, errorMessage);
	case Property::Type::kWord:
		return tryToWord(property, errorMessage);
	case Property::Type::kObject:
		return tryToObject(property, errorMessage);
	case Property::Type::kStruct:
		return tryToStruct(property, reader, origin, errorMessage);
	default:
		errorMessage = "conversion to this type not possible";
		return PropertyPtr();
	}
}

}
}
}
