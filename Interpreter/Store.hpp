#pragma once

#include "FirstRound/Module.hpp"

namespace SRBT
{
namespace Interpret
{
	class Store final
	{
	public:
		using map_t = std::map<std::string, FR::PropertyPtr>;

	public:
		Store(FR::Origin const &origin) : _origin(origin) {}

		FR::PropertyPtr const* find(std::string const &) const;
		FR::Origin const &origin() const { return _origin; }

		bool insert_unique(std::string const &, FR::PropertyPtr const &);

	private:
		map_t _map;
		FR::Origin const &_origin;
	};
}
}
