#pragma once

#include "Utils/Macros.hpp"
#include "Utils/Sharedpointer.hpp"

#include <map>
#include <string>
#include <vector>

namespace SRBT
{
namespace Compiler
{
	class Module;
	class Property;
	MAKE_SHARED_PTR(Property);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	enum struct PrimitiveType
	{
		kBool,
		kInteger,
		kReal,
		kWord,
		kObject,
		kStruct,
		kLinearContainer,
		kCustom = 100,
	};

	using CompleteType = std::vector<PrimitiveType> const;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Property
	{
	public:
		virtual ~Property() = default;

		/**
		 * @brief argument is allways shared pointer to this,
		 * may be set to a new Property
		 */
		virtual void reduce(PropertyPtr & ioSelf) {}

		virtual bool constant() const = 0;
		virtual CompleteType const &type() const = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename type_def>
	class ValueProperty : public Property
	{	
	public:
		using value_t = typename type_def::value_t;
		using in_t = typename type_def::in_t;
		using return_t = typename type_def::return_t;
		using completetype_return_t = typename type_def::completetype_return_t;
		using ptr_t = std::shared_ptr<ValueProperty>;

	public:
		virtual return_t value() const = 0;

	public:
		virtual void reduce(ptr_t & ioSelf) {}

	private:
		/*override from Property*/
		void reduce(PropertyPtr & ioSelf) override final
		{
			ptr_t ptr = Utils::sam_safe_shared_cast<ValueProperty>(ioSelf, __FILE__, __LINE__);
			reduce(ptr);
			ioSelf = ptr;
		}

		CompleteType const &type() const override { return completetype_return_t::ret(value()); }
	};

	template<typename val, typename in, typename ret, typename completetype_return, typename value_return>
	struct Typedef
	{
		using value_t = val;
		using in_t = in;
		using return_t = ret;
		using completetype_return_t = completetype_return;
		using value_return_t = value_return;
		using ptr_t = std::shared_ptr<ValueProperty<Typedef>>;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace prvt
	{
		template<typename in, typename out>
		struct value_return
		{
			inline static out ret(in value)
			{
				return value;
			}
		};

		template<PrimitiveType type, typename in>
		struct value_completetype_return
		{
			inline static CompleteType &ret(in)
			{
				static CompleteType _sType{type};
				return _sType;
			}
		};
	}

	using BTypeDef = Typedef<bool, bool, bool, prvt::value_completetype_return<PrimitiveType::kBool, bool>, prvt::value_return<bool, bool>>;
	using ITypeDef = Typedef<int, int, int, prvt::value_completetype_return<PrimitiveType::kInteger, int>, prvt::value_return<int, int>>;
	using RTypeDef = Typedef<double, double, double, prvt::value_completetype_return<PrimitiveType::kReal, double>, prvt::value_return<double, double>>;
	using WTypeDef = Typedef<std::string, std::string const &, std::string, prvt::value_completetype_return<PrimitiveType::kWord, std::string const&>, prvt::value_return<std::string const&, std::string const &>>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	using BProperty = ValueProperty<BTypeDef>;
	using IProperty = ValueProperty<ITypeDef>;
	using RProperty = ValueProperty<RTypeDef>;
	using WProperty = ValueProperty<WTypeDef>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Object final
	{
	public:
		explicit Object(Module const & module) : _module(module) {}

		Module const & module() const { return _module; }

	private:
		Module const & _module;
	};
	using ObjectPtr = std::unique_ptr<Object>;

	namespace prvt
	{
		struct object_return
		{
			inline static Object & ret(ObjectPtr const &o)
			{
				return *o;
			}
		};

		struct object_completetype_return
		{
			static CompleteType const &ret(Object const &o);
		};
	}

	using OTypeDef = Typedef<ObjectPtr, ObjectPtr, Object &, prvt::object_completetype_return, prvt::object_return>;
	using OProperty = ValueProperty<OTypeDef>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class LinearContainer : public Property
	{
	public:
		LinearContainer(CompleteType const &completeType) : _completeType(completeType) {}

		/*override from Property*/
		bool constant() const override { return false; }
		CompleteType const &type() const override { return _completeType; }

	private:
		CompleteType _completeType;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MAKE_SHARED_PTR(BProperty);
	MAKE_SHARED_PTR(IProperty);
	MAKE_SHARED_PTR(RProperty);
	MAKE_SHARED_PTR(WProperty);
	MAKE_SHARED_PTR(OProperty);
}
}
