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
	if(property->type().size() == 1)
	{
		switch(property->type().front())
		{
		case PrimitiveType::kStruct:
			return std::static_pointer_cast<StructCProperty>(property);
		case PrimitiveType::kWord:
			if(property->constant())
			{
				WProperty::value_t const & word = static_cast<WProperty const &>(*property).value();
				std::experimental::filesystem::path path = origin.path().parent_path();
				path /= word;
				return StructCProperty::instance(reader.read(path));
			}
		default:
			break;
		}
	}

	errorMessage = std::string("cannot convert to ") + Name<StructProperty>();
	return StructPropertyPtr();
}

OPropertyPtr tryToObject(PropertyPtr const & property, std::string & errorMessage)
{
	if(property->type().size() == 1)
	{
		switch(property->type().front())
		{
		case PrimitiveType::kObject:
			return std::static_pointer_cast<OProperty>(property);
		case PrimitiveType::kStruct:
		{
			StructProperty const & s = static_cast<StructProperty const &>(*property);
			return OPropertyPtr(new OCProperty(ObjectPtr(new Object(s.value()))));
		}
		default:
			break;
		}
	}

	errorMessage = std::string("cannot convert to ") + Name<OProperty>();
	return OPropertyPtr();
}

BPropertyPtr tryToBool(PropertyPtr const & property, std::string & errorMessage)
{
	if(property->type().size() == 1)
	{
		switch(property->type().front())
		{
		case PrimitiveType::kBool:
			return std::static_pointer_cast<BProperty>(property);
		case PrimitiveType::kInteger:
			return BPropertyPtr(new Operators::ItoBProperty(std::static_pointer_cast<IProperty>(property)));
		case PrimitiveType::kReal:
			return BPropertyPtr(new Operators::RtoBProperty(std::static_pointer_cast<RProperty>(property)));
		case PrimitiveType::kWord:
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
			break;
		}
	}

	errorMessage = std::string("cannot convert to ") + Name<BProperty>();
	return BPropertyPtr();
}

IPropertyPtr tryToInteger(PropertyPtr const & property, std::string & errorMessage)
{
	if(property->type().size() == 1)
	{
		switch(property->type().front())
		{
		case PrimitiveType::kInteger:
			return std::static_pointer_cast<IProperty>(property);
		case PrimitiveType::kBool:
			return IPropertyPtr(new Operators::BtoIProperty(std::static_pointer_cast<BProperty>(property)));
		case PrimitiveType::kReal:
			return IPropertyPtr(new Operators::RtoIProperty(std::static_pointer_cast<RProperty>(property)));
		case PrimitiveType::kWord:
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
			break;
		}
	}

	errorMessage = std::string("cannot convert to ") + Name<IProperty>();
	return IPropertyPtr();
}

RPropertyPtr tryToReal(PropertyPtr const & property, std::string & errorMessage)
{
	if(property->type().size() == 1)
	{
		switch(property->type().front())
		{
		case PrimitiveType::kReal:
			return std::static_pointer_cast<RProperty>(property);
		case PrimitiveType::kBool:
			return RPropertyPtr(new Operators::BtoRProperty(std::static_pointer_cast<BProperty>(property)));
		case PrimitiveType::kInteger:
			return RPropertyPtr(new Operators::ItoRProperty(std::static_pointer_cast<IProperty>(property)));
		case PrimitiveType::kWord:
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
			break;
		}
	}

	errorMessage = std::string("cannot convert to ") + Name<RProperty>();
	return RPropertyPtr();
}

WPropertyPtr tryToWord(PropertyPtr const & property, std::string & errorMessage)
{
	if(property->type().size() == 1)
	{
		switch(property->type().front())
		{
		case PrimitiveType::kWord:
			return std::static_pointer_cast<WProperty>(property);
		case PrimitiveType::kBool:
			return WPropertyPtr(new Operators::BtoWProperty(std::static_pointer_cast<BProperty>(property)));
		case PrimitiveType::kInteger:
			return WPropertyPtr(new Operators::ItoWProperty(std::static_pointer_cast<IProperty>(property)));
		case PrimitiveType::kReal:
			return WPropertyPtr(new Operators::RtoWProperty(std::static_pointer_cast<RProperty>(property)));
		default:
			break;
		}
	}

	errorMessage = std::string("cannot convert to ") + Name<WProperty>();
	return WPropertyPtr();
}

PropertyPtr tryTo(PrimitiveType const type, PropertyPtr const & property, ModuleReader const & reader, Compiler::FileOrigin const & origin, std::string & errorMessage)
{
	if(property->type().size() == 1)
	{
		switch(property->type().front())
		{
		case PrimitiveType::kBool:
			return tryToBool(property, errorMessage);
		case PrimitiveType::kInteger:
			return tryToInteger(property, errorMessage);
		case PrimitiveType::kReal:
			return tryToReal(property, errorMessage);
		case PrimitiveType::kWord:
			return tryToWord(property, errorMessage);
		case PrimitiveType::kObject:
			return tryToObject(property, errorMessage);
		case PrimitiveType::kStruct:
			return tryToStruct(property, reader, origin, errorMessage);
		default:
			break;
		}
	}

	errorMessage = "conversion to this type not possible";
	return PropertyPtr();
}

}
}
}
