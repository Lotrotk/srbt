#pragma once

#include "Utils/Sharedpointer.hpp"

#include <map>
#include <string>

namespace SRBT
{
namespace Compiler
{
	class Module;
	class Property;
	MAKE_SHARED_PTR(Property);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace prvt
	{
		template<typename value_t, typename return_t>
		struct value_return
		{
			inline static return_t ret(value_t const & value)
			{
				return value;
			}
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Property
	{
	public:
		enum struct Type
		{
			kBool,
			kInteger,
			kReal,
			kWord,
			kObject,
			kStruct,
			kUndefined,
		};

	public:
		virtual ~Property() = default;

		/**
		 * @brief argument is allways shared pointer to this,
		 * may be set to a new Property
		 */
		virtual void reduce(PropertyPtr & ioSelf) {}

	/*abstract*/
	public:
		virtual bool constant() const = 0;
		virtual Type type() const = 0;
		virtual bool dependend_on_property(Module const &, std::string const & name) const = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename type_def>
	class ValueProperty
	:
		public Property
	{
	public:
		static constexpr Type type_v = type_def::type_v;
		using value_t = typename type_def::value_t;
		using in_t = typename type_def::in_t;
		using return_t = typename type_def::return_t;
		using ptr_t = std::shared_ptr<ValueProperty>;

	/*override from Property*/
	public:
		Type type() const override final { return type_v; }

		void reduce(PropertyPtr & ioSelf) override final
		{
			ptr_t ptr = Utils::sam_safe_shared_cast<ValueProperty>(ioSelf, __FILE__, __LINE__);
			reduce(ptr);
			ioSelf = ptr;
		}

		virtual void reduce(ptr_t & ioSelf) {}

	/*abstract*/
	public:
		virtual return_t value() const = 0;
	};

	template<Property::Type typ, typename val, typename in, typename ret, typename value_return>
	struct Typedef
	{
		static constexpr Property::Type type_v = typ;
		using value_t = val;
		using in_t = in;
		using return_t = ret;
		using value_return_t = value_return;
		using ptr_t = std::shared_ptr<ValueProperty<Typedef>>;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	using BTypeDef = Typedef<Property::Type::kBool, bool, bool, bool, prvt::value_return<bool, bool>>;
	using ITypeDef = Typedef<Property::Type::kInteger, int, int, int, prvt::value_return<int, int>>;
	using RTypeDef = Typedef<Property::Type::kReal, double, double, double, prvt::value_return<double, double>>;
	using WTypeDef = Typedef<Property::Type::kWord, std::string, std::string const &, std::string, prvt::value_return<std::string, std::string const &>>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	using BProperty = ValueProperty<BTypeDef>;
	using IProperty = ValueProperty<ITypeDef>;
	using RProperty = ValueProperty<RTypeDef>;
	using WProperty = ValueProperty<WTypeDef>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Object
	{
	public:
		using Mapping = std::map<PropertyPtr, PropertyPtr>;

	public:
		explicit Object(Module const & module) : _module(module) {}

		Mapping const & mapping() const { return _mapping; }
		Mapping & mapping() { return _mapping; }

		Module const & module() const { return _module; }

	private:
		Module const & _module;
		Mapping _mapping;
	};
	using ObjectPtr = std::unique_ptr<Object>;

	namespace prvt
	{
		struct object_return
		{
			inline static Object & ret(ObjectPtr const & o)
			{
				return *o;
			}
		};
	}

	using OTypeDef = Typedef<Property::Type::kObject, ObjectPtr, ObjectPtr, Object &, prvt::object_return>;
	using OProperty = ValueProperty<OTypeDef>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MAKE_SHARED_PTR(BProperty);
	MAKE_SHARED_PTR(IProperty);
	MAKE_SHARED_PTR(RProperty);
	MAKE_SHARED_PTR(WProperty);
	MAKE_SHARED_PTR(OProperty);
}
}
