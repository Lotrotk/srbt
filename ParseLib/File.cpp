#include "File.hpp"

#include <fstream>

using namespace SRBT;
using namespace SRBT::Parse;

bool TextFile::open(path_t const &path)
{
	_buffer.clear();

	std::fstream ifs;
	ifs.open(path.string(), std::ios_base::in | std::ios_base::binary);
	if(!ifs.is_open())
	{
		return false;
	}
	ifs.seekg(0, std::ios_base::end);
	size_t size = ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);
	size -= ifs.tellg();

	_buffer.resize(size);
	ifs.read(_buffer.data(), size);

	return true;
}
