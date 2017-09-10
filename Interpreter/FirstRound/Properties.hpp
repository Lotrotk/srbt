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

	class CompleteType
	{
	public:
		virtual ~CompleteType() = default;

		virtual bool operator==(CompleteType const &) const = 0;
	};
	using CompleteTypePtr = std::shared_ptr<CompleteType>;

	class BasicCompleteType : public CompleteType
	{
	public:
		BasicCompleteType(PrimitiveType const primitive) : _primitive(primitive) {}

		bool operator==(BasicCompleteType const &) const;

		/*override from CompleteType*/
		bool operator==(CompleteType const &) const override;

	private:
		const PrimitiveType _primitive;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Property
	{
	public:
		virtual ~Property() = default;

	protected:
		Property() = default;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<PrimitiveType P, typename constant_type>
	class BasicProperty : public Property
	{
	public:
		/*abstract*/
		virtual constant_type *tryAsConstant() { return nullptr; }

	protected:
		BasicProperty() = default;

	public:
		static std::shared_ptr<BasicCompleteType> const type;
	};

	class BCProperty;
	using BProperty = BasicProperty<PrimitiveType::kBool, BCProperty>;
	using BPropertyPtr = std::shared_ptr<BProperty>;
	class ICProperty;
	using IProperty = BasicProperty<PrimitiveType::kInteger, ICProperty>;
	using IPropertyPtr = std::shared_ptr<IProperty>;
	class RCProperty;
	using RProperty = BasicProperty<PrimitiveType::kReal, RCProperty>;
	using RPropertyPtr = std::shared_ptr<RProperty>;
	class SCProperty;
	using SProperty = BasicProperty<PrimitiveType::kString, SCProperty>;
	using SPropertyPtr = std::shared_ptr<SProperty>;
	class TCProperty;
	using TProperty = BasicProperty<PrimitiveType::kType, TCProperty>;
	using TPropertyPtr = std::shared_ptr<TProperty>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class ObjectProperty : public Property
	{
	public:
		ObjectProperty(Module &module) {}//: _module(module) {}

	private:
		//Module &_module;
	};
	using ObjectPropertyPtr= std::shared_ptr<ObjectProperty>;

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

	using int_t = int;
	using real_t = double;

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
	class TCProperty final : public BasicConstantProperty<TProperty, CompleteTypePtr, CompleteTypePtr const&, CompleteTypePtr const&>
	{
	public:
		using BasicConstantProperty<TProperty, CompleteTypePtr, CompleteTypePtr const&, CompleteTypePtr const&>::BasicConstantProperty;

		/*override from BasicProperty*/
		TCProperty *tryAsConstant() override { return this; }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class ReferencedProperty final : public Property
	{
	public:
		ReferencedProperty(std::string &&ref_name) : _ref_name{std::move(ref_name)} {}

		std::list<std::string> const& reference_name() const { return _ref_name; }

		void merge(ReferencedProperty &&continuation);

	private:
		std::list<std::string> _ref_name;
	};
}
}
}
