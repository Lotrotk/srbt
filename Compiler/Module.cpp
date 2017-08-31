#include "Module.hpp"

using namespace SRBT;
using namespace Compiler;

using namespace std::experimental;

bool FileOrigin::operator==(Origin const & origin) const
{
	if(FileOrigin const * const fileOrigin = dynamic_cast<FileOrigin const *>(&origin))
	{
		return *fileOrigin == *this;
	}

	return false;
}

bool FileOrigin::operator==(FileOrigin const & origin) const
{
	try
	{
		return filesystem::equivalent(_path, origin._path);
	}
	catch(filesystem::filesystem_error const & exc)
	{
		throw Utils::TechnicalException(exc.what());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool StructOrigin::operator==(StructOrigin const &o) const
{
	return _line == o._line && *_fileOrigin == *o._fileOrigin;
}

bool StructOrigin::operator==(Origin const &o) const
{
	if(StructOrigin const *const structOrigin = dynamic_cast<StructOrigin const*>(&o))
	{
		return *structOrigin == *this;
	}

	return false;
}

std::string StructOrigin::toString() const
{
	return _fileOrigin->path().u8string() + ':' + std::to_string(_line);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Module::Member::Member(PropertyPtr const & property) : _type(property->type()), _property(property)
{
	if(!_property)
	{
		throw Utils::Unexpected(__FILE__, __LINE__, "invalid argument");
	}
}

Module::Member::Member(BMPropertyPtr const &property) : Member(PropertyPtr(property))
{
}

Module::Member::Member(IMPropertyPtr const &property) : Member(PropertyPtr(property))
{
}

Module::Member::Member(RMPropertyPtr const &property) : Member(PropertyPtr(property))
{
}

Module::Member::Member(OMPropertyPtr const &property) : Member(PropertyPtr(property))
{
}

Module::Member::Member(WMPropertyPtr const &property) : Member(PropertyPtr(property))
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

id_t Module::_sModuleID{id_t(PrimitiveType::kCustom)};
