#include "Module.hpp"

using namespace SRBT;
using namespace Compiler;

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
