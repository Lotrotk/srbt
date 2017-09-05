#include "Store.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;

FR::PropertyPtr const*Store::find(std::string const &name) const
{
	map_t::const_iterator const it = _map.find(name);
	return it == _map.cend() ? nullptr : &it->second;
}

bool Store::insert_unique(std::string const &name, FR::PropertyPtr const &property)
{
	return _map.insert(map_t::value_type(name, property)).second;
}
