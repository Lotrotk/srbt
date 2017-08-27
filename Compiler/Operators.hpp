#pragma once

#include "Operators_Unary.hpp"

namespace SRBT
{
namespace Compiler
{
namespace Operators
{
	namespace prvt
	{
		template<typename type_def, typename type_def_in>
		struct Arithmetic;

		template<>
		struct Arithmetic<BTypeDef, BTypeDef>
		{
			enum struct Operation
			{
				kAnd,
				kOr,
				kXor,
				kEq,
			};

			static constexpr BProperty::return_t operate(BProperty::in_t const arg0, BProperty::in_t const arg1, Operation const op)
			{
				switch(op)
				{
				case Operation::kAnd:
					return arg0 && arg1;
				case Operation::kOr:
					return arg0 || arg1;
				case Operation::kXor:
					return arg0 != arg1;
				default:
					return arg0 == arg1;
				}
			}

			static BPropertyPtr reduced(BPropertyPtr arg0, BPropertyPtr arg1, Operation const op)
			{
				BPropertyPtr res;

				if(arg1->constant())
				{
					using std::swap;
					swap(arg0, arg1);
				}
				if(arg0->constant())
				{
					BProperty::value_t const a0 = arg0->value();
					switch(op)
					{
					case Operation::kAnd:
						res = (a0 ? arg1 : BCProperty::instance(false));
						break;
					case Operation::kOr:
						res = (a0 ? BCProperty::instance(true) : arg1);
						break;
					case Operation::kXor:
						res = (a0 ? BPropertyPtr(new NotBProperty(arg1)) : arg1);
						break;
					default:
						res = (a0 ? arg1 : BPropertyPtr(new NotBProperty(arg1)));
						break;
					}
				}
				else if(arg0 == arg1)
				{
					switch(op)
					{
						case Operation::kAnd:
						case Operation::kOr:
							res = arg0;
							break;
						case Operation::kXor:
							res = BCProperty::instance(false);
							break;
						default:
							res = BCProperty::instance(true);
							break;
					}
				}

				if(res.get())
				{
					res->reduce(res);
				}

				return res;
			}
		};

		template<>
		struct Arithmetic<ITypeDef, ITypeDef>
		{
			enum struct Operation
			{
				kPlus,
				kMinus,
				kMul,
				kDiv,
				kModulo,
			};

			static constexpr IProperty::return_t operate(IProperty::in_t const arg0, IProperty::in_t const arg1, Operation const op)
			{
				switch(op)
				{
				case Operation::kPlus:
					return arg0 + arg1;
				case Operation::kMinus:
					return arg0 - arg1;
				case Operation::kMul:
					return arg0 * arg1;
				case Operation::kDiv:
					return arg0 / arg1;
				default:
					return arg0 % arg1;
				}
			}

			static IPropertyPtr reduced(IPropertyPtr const & arg0, IPropertyPtr const & arg1, Operation const op)
			{
				IPropertyPtr res;

				if(arg0->constant())
				{
					IProperty::value_t const a0 = arg0->value();
					if(a0 == 0)
					{
						switch(op)
						{
						case Operation::kPlus:
							res = arg1;
							break;
						case Operation::kMinus:
							res.reset(new MinusIProperty(arg1));
							break;
						default:
							res = ICProperty::instance(0);
							break;
						}
					}
					else if(a0 == 1)
					{
						if(op == Operation::kMul)
						{
							res = arg1;
						}
					}
				}
				else if(arg1->constant())
				{
					IProperty::value_t const a1 = arg1->value();
					if(a1 == 0)
					{
						switch(op)
						{
						case Operation::kPlus:
						case Operation::kMinus:
							res = arg0;
							break;
						default:
							res = ICProperty::instance(0);
							break;
						}
					}
					else if(a1 == 1)
					{
						switch(op)
						{
						case Operation::kMul:
						case Operation::kDiv:
							res = arg0;
							break;
						case Operation::kModulo:
							res = ICProperty::instance(0);
							break;
						default:
							break;
						}
					}
				}
				else if(arg0 == arg1)
				{
					switch(op)
					{
					case Operation::kMinus:
						res = ICProperty::instance(0);
						break;
					case Operation::kDiv:
						res = ICProperty::instance(1);
						break;
					default:
						break;
					}
				}

				if(res.get())
				{
					res->reduce(res);
				}

				return res;
			}
		};

		template<>
		struct Arithmetic<RTypeDef, RTypeDef>
		{
			enum struct Operation
			{
				kPlus,
				kMinus,
				kMul,
				kDiv,
			};

			static constexpr RProperty::return_t operate(RProperty::in_t const arg0, RProperty::in_t const arg1, Operation const op)
			{
				switch(op)
				{
				case Operation::kPlus:
					return arg0 + arg1;
				case Operation::kMinus:
					return arg0 - arg1;
				case Operation::kMul:
					return arg0 * arg1;
				default:
					return arg0 / arg1;
				}
			}

			static RPropertyPtr reduced(RPropertyPtr const & arg0, RPropertyPtr const & arg1, Operation const op)
			{
				RPropertyPtr res;

				if(arg0->constant())
				{
					RProperty::value_t const a0 = arg0->value();
					if(a0 == 0.)
					{
						switch(op)
						{
						case Operation::kPlus:
							res = arg1;
							break;
						case Operation::kMinus:
							res.reset(new MinusRProperty(arg1));
							break;
						default:
							res = RCProperty::instance(0.);
							break;
						}
					}
					else if(a0 == 1.)
					{
						if(op == Operation::kMul)
						{
							res = arg1;
						}
					}
				}
				else if(arg1->constant())
				{
					RProperty::value_t const a1 = arg1->value();
					if(a1 == 0.)
					{
						switch(op)
						{
						case Operation::kPlus:
						case Operation::kMinus:
							res = arg0;
							break;
						default:
							res = RCProperty::instance(0);
							break;
						}
					}
					else if(a1 == 1.)
					{
						switch(op)
						{
						case Operation::kMul:
						case Operation::kDiv:
							res = arg0;
							break;
						default:
							break;
						}
					}
				}
				else if(arg0 == arg1)
				{
					switch(op)
					{
					case Operation::kMinus:
						res = RCProperty::instance(0.);
						break;
					case Operation::kDiv:
						res = RCProperty::instance(1.);
						break;
					default:
						break;
					}
				}

				if(res.get())
				{
					res->reduce(res);
				}

				return res;
			}
		};

		template<typename type_def, typename type_def_in>
		struct Compare;

		template<>
		struct Compare<BTypeDef, ITypeDef>
		{
			enum struct Operation
			{
				kEq,
				kDiff,
				kLess,
				kLeq,
			};

			static constexpr BProperty::return_t operate(IProperty::in_t const arg0, IProperty::in_t const arg1, Operation const op)
			{
				switch(op)
				{
				case Operation::kEq:
					return arg0 == arg1;
				case Operation::kDiff:
					return arg0 != arg1;
				case Operation::kLess:
					return arg0 < arg1;
				default:
					return arg0 <= arg1;
				}
			}

			static BPropertyPtr reduced(IPropertyPtr const & arg0, IPropertyPtr const & arg1, Operation const op)
			{
				if(arg0 == arg1)
				{
					return BCProperty::instance(op == Operation::kEq || op == Operation::kLeq);
				}

				return BPropertyPtr();
			}
		};

		template<>
		struct Compare<BTypeDef, RTypeDef>
		{
			enum struct Operation
			{
				kEq,
				kDiff,
				kLess,
				kLeq,
			};

			static constexpr BProperty::return_t operate(RProperty::in_t const arg0, RProperty::in_t const arg1, Operation const op)
			{
				switch(op)
				{
				case Operation::kEq:
					return arg0 == arg1;
				case Operation::kDiff:
					return arg0 != arg1;
				case Operation::kLess:
					return arg0 < arg1;
				default:
					return arg0 <= arg1;
				}
			}

			static BPropertyPtr reduced(RPropertyPtr const & arg0, RPropertyPtr const & arg1, Operation const op)
			{
				if(arg0 == arg1)
				{
					return BCProperty::instance(op == Operation::kEq || op == Operation::kLeq);
				}

				return BPropertyPtr();
			}
		};

		template<>
		struct Compare<BTypeDef, WTypeDef>
		{
			enum struct Operation
			{
				kEq,
				kDiff,
			};

			static BProperty::return_t operate(WProperty::in_t arg0, WProperty::in_t arg1, Operation const op)
			{
				return (op == Operation::kEq) == (arg0 == arg1);
			}

			static BPropertyPtr reduced(WPropertyPtr const & arg0, WPropertyPtr const & arg1, Operation const op)
			{
				if(arg0 == arg1)
				{
					return BCProperty::instance(op == Operation::kEq);
				}

				return BPropertyPtr();
			}
		};

		template<typename type_def, typename type_def_in, template<typename out, typename in> class binary_operator>
		class BinaryOperator final
		:
			public ValueProperty<type_def>
		{
		public:
			using return_t = typename type_def::return_t;
			using ptr_t = typename ValueProperty<type_def>::ptr_t;

			using value_in_t = typename type_def::value_t;
			using in_in_t = typename type_def::in_t;
			using ptr_t_in = typename ValueProperty<type_def_in>::ptr_t;

			using Binary_operator = binary_operator<type_def, type_def_in>;
			using Operation = typename Binary_operator::Operation;

		public:
			explicit BinaryOperator(ptr_t_in const & arg0, ptr_t_in const & arg1, Operation const op) : _arg0(arg0), _arg1(arg1), _operation(op) {}

		/*override from Property*/
		public:
			bool constant() const override { return _arg0->constant() && _arg1->constant(); }
			bool dependend_on_property(Module const & module, std::string const & name) const override
			{
				return _arg0->dependend_on_property(module, name) || _arg1->dependend_on_property(module, name);
			}

		/*override from ValueProperty*/
		public:
			return_t value() const override { return Binary_operator::operate(_arg0->value(), _arg1->value(), _operation); }

			void reduce(ptr_t & ioSelf) override
			{
				_arg0->reduce(_arg0);
				_arg1->reduce(_arg1);
				if(constant())
				{
					ioSelf.reset(new ConstantProperty<type_def>(value()));
				}
				else if(ptr_t const reduced = Binary_operator::reduced(_arg0, _arg1, _operation))
				{
					ioSelf = reduced;
				}
			}

		private:
			ptr_t_in _arg0;
			ptr_t_in _arg1;
			Operation _operation;
		};
	}

	using BLogic = prvt::Arithmetic<BTypeDef, BTypeDef>::Operation;
	using LogicBProperty = prvt::BinaryOperator<BTypeDef, BTypeDef, prvt::Arithmetic>;
	using IArithmatic = prvt::Arithmetic<ITypeDef, ITypeDef>::Operation;
	using ArithmIProperty = prvt::BinaryOperator<ITypeDef, ITypeDef, prvt::Arithmetic>;
	using RArithmatic = prvt::Arithmetic<RTypeDef, RTypeDef>::Operation;
	using ArithmRProperty = prvt::BinaryOperator<RTypeDef, RTypeDef, prvt::Arithmetic>;

	using ILogic = prvt::Compare<BTypeDef, ITypeDef>::Operation;
	using LogicIProperty = prvt::BinaryOperator<BTypeDef, ITypeDef, prvt::Compare>;
	using RLogic = prvt::Compare<BTypeDef, RTypeDef>::Operation;
	using LogicRProperty = prvt::BinaryOperator<BTypeDef, RTypeDef, prvt::Compare>;
	using WLogic = prvt::Compare<BTypeDef, WTypeDef>::Operation;
	using LogicWProperty = prvt::BinaryOperator<BTypeDef, WTypeDef, prvt::Compare>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// binaray operators -- without operand //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace prvt
	{
		template<typename type_def, typename type_def_in>
		struct Concat;

		template<>
		struct Concat<WTypeDef, WTypeDef>
		{
			static WProperty::return_t operate(WProperty::in_t arg0, WProperty::in_t arg1)
			{
				return arg0 + arg1;
			}

			static inline WPropertyPtr reduced(WPropertyPtr const & arg0, WPropertyPtr const & arg1)
			{
				return WPropertyPtr();
			}
		};

		template<typename type_def, typename type_def_in, template<typename out, typename in> class binary_operator>
		class BinaryOperator_without_operand final
		:
			public ValueProperty<type_def>
		{
		public:
			using return_t = typename type_def::return_t;
			using ptr_t = typename ValueProperty<type_def>::ptr_t;

			using value_in_t = typename type_def::value_t;
			using in_in_t = typename type_def::in_t;
			using ptr_t_in = typename ValueProperty<type_def_in>::ptr_t;

			using Binary_operator = binary_operator<type_def, type_def_in>;

		public:
			explicit BinaryOperator_without_operand(ptr_t_in const & arg0, ptr_t_in const & arg1) : _arg0(arg0), _arg1(arg1) {}

		/*override from Property*/
		public:
			bool constant() const override { return _arg0->constant() && _arg1->constant(); }
			bool dependend_on_property(Module const & module, std::string const & name) const override
			{
				return _arg0->dependend_on_property(module, name) || _arg1->dependend_on_property(module, name);
			}

		/*override from ValueProperty*/
		public:
			return_t value() const override { return Binary_operator::operate(_arg0->value(), _arg1->value()); }

			void reduce(ptr_t & ioSelf) override
			{
				_arg0->reduce(_arg0);
				_arg1->reduce(_arg1);
				if(constant())
				{
					ioSelf.reset(new ConstantProperty<type_def>(value()));
				}
				else if(ptr_t const reduced = Binary_operator::reduced(_arg0, _arg1))
				{
					ioSelf = reduced;
				}
			}

		private:
			ptr_t_in _arg0;
			ptr_t_in _arg1;
		};
	}

	using ConcatProperty = prvt::BinaryOperator_without_operand<WTypeDef, WTypeDef, prvt::Concat>;
}
}
}
