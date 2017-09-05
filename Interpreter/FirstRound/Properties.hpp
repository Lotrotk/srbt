#pragma once

#include "Utils/Macros.hpp"

#include <list>
#include <memory>
#include <string>

namespace SRBT
{
namespace Interpret
{
namespace FR
{
	class Module;
	class Property;
	using ModulePtr = std::shared_ptr<Module>;
	using PropertyPtr = std::shared_ptr<Property>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	enum struct PrimitiveType
	{
		kBool,
		kInteger,
		kReal,
		kString,
		kObject,
		kType,
		kLinearContainer,
		kNo_custom_type,
		kCustom = 100,
	};

	using CompleteType = std::list<PrimitiveType>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Property
	{
	public:
		virtual ~Property() = default;

		bool isType(PrimitiveType const p) const { return isType(CompleteType{p}); }

		/*abrstract*/
		virtual CompleteType getType() const = 0;
		virtual bool isType(CompleteType const &) const = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	using int_t = int;
	using real_t = double;

	template<typename T, typename constant_type, PrimitiveType P>
	class BasicProperty : public Property
	{
	public:
		/*override from Property*/
		CompleteType getType() const override { return type(); }
		bool isType(CompleteType const &t) const override { return t.size() == 1 && t.front() == P; }

		static CompleteType const &type() { static CompleteType const t{P}; return t; }

		/*abstract*/
		virtual constant_type *tryAsConstant() { return nullptr; }

	protected:
		BasicProperty() = default;
	};

	class BCProperty;
	using BProperty = BasicProperty<bool, BCProperty, PrimitiveType::kBool>;
	using BPropertyPtr = std::shared_ptr<BProperty>;
	class ICProperty;
	using IProperty = BasicProperty<int_t, ICProperty, PrimitiveType::kInteger>;
	using IPropertyPtr = std::shared_ptr<IProperty>;
	class RCProperty;
	using RProperty = BasicProperty<real_t, RCProperty, PrimitiveType::kReal>;
	using RPropertyPtr = std::shared_ptr<RProperty>;
	class SCProperty;
	using SProperty = BasicProperty<std::string, SCProperty, PrimitiveType::kString>;
	using SPropertyPtr = std::shared_ptr<SProperty>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class ObjectProperty : public Property
	{
	public:
		ObjectProperty(Module &module) : _module(module) {}

		/*override from Property*/
		CompleteType getType() const override;
		bool isType(CompleteType const &t) const override;

	private:
		Module &_module;
	};
	using ObjectPropertyPtr= std::shared_ptr<ObjectProperty>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class TypeProperty : public Property
	{
	public:
		TypeProperty(CompleteType const &t) : _myType(t) {}

		/*override from Property*/
		CompleteType getType() const override;
		bool isType(CompleteType const &t) const override;

	private:
		CompleteType _myType;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename parent, typename v, typename in, typename out>
	class BasicConstantProperty : public parent
	{
	public:
		BasicConstantProperty(v value) : _value(std::move(value)) {}

		out value() const { return _value; }

	private:
		v const _value;
	};

	class BCProperty final : public BasicConstantProperty<BProperty, bool, bool, bool>
	{
	public:
		using BasicConstantProperty<BProperty, bool, bool, bool>::BasicConstantProperty;

		/*override from BasicProperty*/
		BCProperty *tryAsConstant() override { return this; }
	};
	class ICProperty final : public BasicConstantProperty<IProperty, int_t, int_t, int_t>
	{
	public:
		using BasicConstantProperty<IProperty, int_t, int_t, int_t>::BasicConstantProperty;

		/*override from BasicProperty*/
		ICProperty *tryAsConstant() override { return this; }
	};
	class RCProperty final : public BasicConstantProperty<RProperty, real_t, real_t, real_t>
	{
	public:
		using BasicConstantProperty<RProperty, real_t, real_t, real_t>::BasicConstantProperty;

		/*override from BasicProperty*/
		RCProperty *tryAsConstant() override { return this; }
	};
	class SCProperty final : public BasicConstantProperty<SProperty, std::string, std::string&&, std::string const&>
	{
	public:
		using BasicConstantProperty<SProperty, std::string, std::string&&, std::string const&>::BasicConstantProperty;

		/*override from BasicProperty*/
		SCProperty *tryAsConstant() override { return this; }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename parent>
	class ReferencedProperty : public parent
	{
	public:
		ReferencedProperty(std::string &&ref_name) : _ref_name(std::move(ref_name)) {}

		std::string const& reference_name() const { return _ref_name; }

	private:
		std::string _ref_name;
	};

	using BRProperty = ReferencedProperty<BProperty>;
	using SRProperty = ReferencedProperty<SProperty>;
}
}
}
