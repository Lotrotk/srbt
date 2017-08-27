#pragma once

#include "Properties.hpp"

namespace SRBT
{
namespace Compiler
{
	template<typename type_def>
	class MutableProperty final
	:
		public ValueProperty<type_def>
	{
	public:
		using value_t = typename type_def::value_t;
		using in_t = typename type_def::in_t;
		using return_t = typename type_def::return_t;
		using value_return_t = typename type_def::value_return_t;
		using ptr_t = std::shared_ptr<ValueProperty<type_def>>;
		using delegate_t = ValueProperty<type_def>;
		using delegate_ptr_t = std::shared_ptr<delegate_t>;

	public:
		explicit MutableProperty(delegate_ptr_t const & value) : _value(value) {}

		void setEverReassigned() { _isEverReassigned = true; }

	/*override from Property*/
	public:
		bool constant() const override { return !_isEverReassigned; }
		bool dependend_on_property(Module const & module, std::string const & name) const override { return _value->dependend_on_property(module, name); }
		void reduce(ptr_t & ioSelf) override
		{
			_value->reduce(_value);
			if(_isEverReassigned) { ioSelf = _value; }
		}

	/*override from ValueProperty*/
	public:
		return_t value() const override { return _value->value(); }

	private:
		delegate_ptr_t _value;
		bool _isEverReassigned = false;
	};

	using BMProperty = MutableProperty<BTypeDef>;
	using IMProperty = MutableProperty<ITypeDef>;
	using RMProperty = MutableProperty<RTypeDef>;
	using WMProperty = MutableProperty<WTypeDef>;
	using OMProperty = MutableProperty<OTypeDef>;

	MAKE_SHARED_PTR(BMProperty);
	MAKE_SHARED_PTR(IMProperty);
	MAKE_SHARED_PTR(RMProperty);
	MAKE_SHARED_PTR(WMProperty);
	MAKE_SHARED_PTR(OMProperty);
}
}
