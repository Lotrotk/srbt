#pragma once

#include "Compiler/Module.hpp"

namespace SRBT
{
namespace Interpret
{
	class Store final
	{
	public:
		using map_t = std::map<std::string, Compiler::PropertyPtr>;

	public:
		Store(Compiler::Origin const &origin) : _origin(origin) {}

		Compiler::PropertyPtr const* find(std::string const &) const;
		Compiler::Origin const &origin() const { return _origin; }

		bool insert_unique(std::string const &, Compiler::PropertyPtr const &);

	private:
		map_t _map;
		Compiler::Origin const &_origin;
	};
}
}
