#pragma once

#include "Properties.hpp"

#include <map>

namespace SRBT
{
namespace Compiler
{
	template<typename type_def>
	class ConstantProperty final
	:
		public ValueProperty<type_def>
	{
	public:
		using value_t = typename type_def::value_t;
		using in_t = typename type_def::in_t;
		using return_t = typename type_def::return_t;
		using value_return_t = typename type_def::value_return_t;

	public:
		explicit ConstantProperty(in_t value) : _value(std::forward<in_t>(value)) {}

	/*override from Property*/
	public:
		bool constant() const override { return true; }

	/*override from ValueProperty*/
	public:
		return_t value() const override { return value_return_t::ret(_value); }

	private:
		value_t _value;
	};

	namespace prvt
	{
		template<typename type_def>
		struct KeyForConstantCachedProperty
		{
			using key_t = typename type_def::value_t;
			static inline key_t key(typename type_def::in_t const value)
			{
				return value;
			}
		};
	}

	template<typename type_def>
	class ConstantCachedProperty final
	:
		public ValueProperty<type_def>
	{
	public:
		using value_t = typename type_def::value_t;
		using in_t = typename type_def::in_t;
		using return_t = typename type_def::return_t;
		using key_t = prvt::KeyForConstantCachedProperty<type_def>;

	private:
		using ptr_t = std::shared_ptr<ConstantCachedProperty>;
		using Cache = std::map<typename key_t::key_t, ptr_t>;

	public:
		static ptr_t const & instance(in_t const value)
		{
			ptr_t & res = _cache[key_t::key(value)];
			if(!res)
			{
				res.reset(new ConstantCachedProperty(value));
			}

			return res;
		}

		static void clearCache()
		{
			_cache = Cache();
		}

	private:
		explicit ConstantCachedProperty(in_t const value) : _value(value) {}

	/*override from Property*/
	public:
		bool constant() const override { return true; }

	/*override from ValueProperty*/
	public:
		return_t value() const override { return _value; }

	private:
		value_t _value;
		static Cache _cache;
	};

	using BCProperty = ConstantCachedProperty<BTypeDef>;
	using ICProperty = ConstantCachedProperty<ITypeDef>;
	using RCProperty = ConstantCachedProperty<RTypeDef>;
	using WCProperty = ConstantCachedProperty<WTypeDef>;
	using OCProperty = ConstantProperty<OTypeDef>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace prvt
	{
		struct struct_completetype_return
		{
			static CompleteType &ret(Module const &);
		};
	}

	using STypeDef = Typedef<Module const &, Module const &, Module const &, prvt::struct_completetype_return, prvt::value_return<Module const &, Module const &>>;
	using StructProperty = ValueProperty<STypeDef>;
	MAKE_SHARED_PTR(StructProperty);

	namespace prvt
	{
		template<>
		struct KeyForConstantCachedProperty<STypeDef>
		{
			using key_t = Module const *;
			static inline key_t key(Module const & value)
			{
				return &value;
			}
		};
	}

	using StructCProperty = ConstantCachedProperty<STypeDef>;
}
}
