#pragma once

#include "Properties.hpp"

namespace SRBT
{
namespace Compiler
{
	class ReferencedPropertyData
	{
	public:
		ReferencedPropertyData(OPropertyPtr const & object, std::string const & member) : _object(object), _member(member) {}

		bool dependend_on_property(Module const &, std::string const & name) const;

	public:
		OPropertyPtr _object;
		std::string _member;
	};

	template<typename type_def>
	class ReferencedProperty final
	:
		public ValueProperty<type_def>
	{
	public:
		using return_t = typename type_def::return_t;

	public:
		ReferencedProperty(OPropertyPtr const & object, std::string const & member) : _data(object, member) {}

		OPropertyPtr const & object() const { return _data._object; }
		std::string const & member() const { return _data._member; }

	/*override from Property*/
	public:
		bool constant() const override { return false; }
		bool dependend_on_property(Module const & module, std::string const & name) const override { return _data.dependend_on_property(module, name); }

	public:
		/*override from ValueProperty*/
		/**
		 * @throws : Unexpected
		 */
		return_t value() const override { throw Utils::Unexpected(__FILE__, __LINE__, "no getter for referenced member"); }

	private:
		ReferencedPropertyData _data;
	};

	using BRProperty = ReferencedProperty<BTypeDef>;
	using IRProperty = ReferencedProperty<ITypeDef>;
	using RRProperty = ReferencedProperty<RTypeDef>;
	using WRProperty = ReferencedProperty<WTypeDef>;
	using ORProperty = ReferencedProperty<OTypeDef>;

	MAKE_SHARED_PTR(BRProperty);
	MAKE_SHARED_PTR(IRProperty);
	MAKE_SHARED_PTR(RRProperty);
	MAKE_SHARED_PTR(WRProperty);
	MAKE_SHARED_PTR(ORProperty);
}
}
