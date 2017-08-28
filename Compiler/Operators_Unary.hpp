#pragma once

#include "ConstantProperties.hpp"

namespace SRBT
{
namespace Compiler
{
namespace Operators
{
	namespace prvt
	{
		template<typename type_def, typename type_def_in>
		struct Negate;

		template<>
		struct Negate<BTypeDef, BTypeDef>
		{
			static inline constexpr BProperty::return_t operate(BProperty::in_t const arg)
			{
				return !arg;
			}

			static BPropertyPtr reduced(BPropertyPtr const & arg);
		};

		template<>
		struct Negate<ITypeDef, ITypeDef>
		{
			static inline constexpr IProperty::return_t operate(IProperty::in_t const arg)
			{
				return -arg;
			}

			static IPropertyPtr reduced(IPropertyPtr const & arg);
		};

		template<>
		struct Negate<RTypeDef, RTypeDef>
		{
			static inline constexpr RProperty::return_t operate(RProperty::in_t const arg)
			{
				return -arg;
			}

			static RPropertyPtr reduced(RPropertyPtr const & arg);
		};

		template<typename type_def, typename type_def_in>
		struct ToBool;

		template<>
		struct ToBool<BTypeDef, ITypeDef>
		{
			static inline constexpr BProperty::return_t operate(IProperty::in_t const arg)
			{
				return arg != IProperty::value_t();
			}

			static BPropertyPtr reduced(IPropertyPtr const & arg) { return BPropertyPtr(); }
		};

		template<>
		struct ToBool<BTypeDef, RTypeDef>
		{
			static inline constexpr BProperty::return_t operate(RProperty::in_t const arg)
			{
				return arg != RProperty::value_t();
			}

			static inline BPropertyPtr reduced(RPropertyPtr const & arg) { return BPropertyPtr(); }
		};

		template<typename type_def, typename type_def_in>
		struct ToInteger;

		template<>
		struct ToInteger<ITypeDef, BTypeDef>
		{
			static inline constexpr IProperty::return_t operate(BProperty::in_t const arg)
			{
				return arg;
			}

			static inline IPropertyPtr reduced(BPropertyPtr const & arg) { return IPropertyPtr(); }
		};

		template<>
		struct ToInteger<ITypeDef, RTypeDef>
		{
			static inline constexpr IProperty::return_t operate(RProperty::in_t const arg)
			{
				return arg;
			}

			static inline IPropertyPtr reduced(RPropertyPtr const & arg) { return IPropertyPtr(); }
		};

		template<typename type_def, typename type_def_in>
		struct ToReal;

		template<>
		struct ToReal<RTypeDef, BTypeDef>
		{
			static inline constexpr RProperty::return_t operate(BProperty::in_t const arg)
			{
				return arg ? RProperty::value_t(1) : RProperty::value_t();
			}

			static inline RPropertyPtr reduced(BPropertyPtr const & arg) { return RPropertyPtr(); }
		};

		template<>
		struct ToReal<RTypeDef, ITypeDef>
		{
			static inline constexpr RProperty::return_t operate(IProperty::in_t const arg)
			{
				return arg;
			}

			static inline RPropertyPtr reduced(IPropertyPtr const & arg) { return RPropertyPtr(); }
		};

		template<typename type_def, typename type_def_in>
		struct ToWord;

		template<>
		struct ToWord<WTypeDef, BTypeDef>
		{
			static inline WProperty::return_t operate(BProperty::in_t const arg)
			{
				return arg ? "true" : "false";
			}

			static inline WPropertyPtr reduced(BPropertyPtr const & arg) { return WPropertyPtr(); }
		};

		template<>
		struct ToWord<WTypeDef, ITypeDef>
		{
			static inline WProperty::return_t operate(IProperty::in_t const arg)
			{
				return std::to_string(arg);
			}

			static inline WPropertyPtr reduced(IPropertyPtr const & arg) { return WPropertyPtr(); }
		};

		template<>
		struct ToWord<WTypeDef, RTypeDef>
		{
			static inline WProperty::return_t operate(RProperty::in_t const arg)
			{
				return std::to_string(arg);
			}

			static inline WPropertyPtr reduced(RPropertyPtr const & arg) { return WPropertyPtr(); }
		};
	}

	template<typename type_def, typename type_def_in, template<typename out, typename in> class unary_operator>
	class UnaryOperator final
	:
		public ValueProperty<type_def>
	{
	public:
		using return_t = typename type_def::return_t;
		using ptr_t = typename ValueProperty<type_def>::ptr_t;

		using value_in_t = typename type_def::value_t;
		using in_in_t = typename type_def::in_t;
		using ptr_t_in = typename ValueProperty<type_def_in>::ptr_t;

		using Unary_operator = unary_operator<type_def, type_def_in>;

	public:
		explicit UnaryOperator(ptr_t_in const & arg) : _arg(arg) {}

	/*override from Property*/
	public:
		bool constant() const override { return _arg->constant(); }

	/*override from ValueProperty*/
	public:
		return_t value() const override { return Unary_operator::operate(_arg->value()); }

		void reduce(ptr_t & ioSelf) override
		{
			_arg->reduce(_arg);
			if(constant())
			{
				ioSelf.reset(new ConstantProperty<type_def>(value()));
			}
			else if(ptr_t const reduced = Unary_operator::reduced(_arg))
			{
				ioSelf = reduced;
			}
		}

	private:
		ptr_t_in _arg;

	private:
		friend struct unary_operator<type_def, type_def_in>;
	};

	using NotBProperty = UnaryOperator<BTypeDef, BTypeDef, prvt::Negate>;
	using MinusIProperty = UnaryOperator<ITypeDef, ITypeDef, prvt::Negate>;
	using MinusRProperty = UnaryOperator<RTypeDef, RTypeDef, prvt::Negate>;

	using ItoBProperty = UnaryOperator<BTypeDef, ITypeDef, prvt::ToBool>;
	using RtoBProperty = UnaryOperator<BTypeDef, RTypeDef, prvt::ToBool>;

	using BtoIProperty = UnaryOperator<ITypeDef, BTypeDef, prvt::ToInteger>;
	using RtoIProperty = UnaryOperator<ITypeDef, RTypeDef, prvt::ToInteger>;

	using BtoRProperty = UnaryOperator<RTypeDef, BTypeDef, prvt::ToReal>;
	using ItoRProperty = UnaryOperator<RTypeDef, ITypeDef, prvt::ToReal>;

	using BtoWProperty = UnaryOperator<WTypeDef, BTypeDef, prvt::ToWord>;
	using ItoWProperty = UnaryOperator<WTypeDef, ITypeDef, prvt::ToWord>;
	using RtoWProperty = UnaryOperator<WTypeDef, RTypeDef, prvt::ToWord>;
}
}
}
