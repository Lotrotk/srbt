#pragma once

#include "FirstRound/Module.hpp"

namespace SRBT
{
namespace Interpret
{
	class Store final
	{
	public:
		struct Value final
		{
			FR::PropertyPtr _property;
			FR::CompleteTypePtr _type;
		};

	public:
		using map_t = std::map<std::string, Value>;

	public:
		Store(FR::Origin const &origin) : _origin(origin) {}

		Value const* find(std::string const &) const;
		FR::Origin const &origin() const { return _origin; }

		bool insert_unique(std::string const &, FR::PropertyPtr const &, FR::CompleteTypePtr const &);

	private:
		map_t _map;
		FR::Origin const &_origin;
	};
}
}
