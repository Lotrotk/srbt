#include "Parser.hpp"

#include "Exception.hpp"
#include "Keys.hpp"

#include "ParseLib/Tree.hpp"

using namespace SRBT;
using namespace SRBT::Parser;

namespace
{
	Utils::RangeEnumerator<Parse::key_t const&, std::vector<Parse::key_t>::const_iterator> sEnumerator_for_operators;
	Utils::RangeEnumerator<Parse::key_t const&, std::vector<Parse::key_t>::const_iterator> sEnumerator_for_keywords;

	struct InitEnumberators
	{
		InitEnumberators()
		{
			_operators.push_back(Parse::key_t{"(", Operators::kBraceLeft});
			_operators.push_back(Parse::key_t{")", Operators::kBraceRight});
			_operators.push_back(Parse::key_t{"{", Operators::kCurlyBraceLeft});
			_operators.push_back(Parse::key_t{"}", Operators::kCurlyBraceRight});
			_operators.push_back(Parse::key_t{"[", Operators::kSquareBraceLeft});
			_operators.push_back(Parse::key_t{"]", Operators::kSquareBraceRight});
			_operators.push_back(Parse::key_t{".", Operators::kPoint});
			_operators.push_back(Parse::key_t{",", Operators::kComma});
			_operators.push_back(Parse::key_t{":", Operators::kColon});
			_operators.push_back(Parse::key_t{";", Operators::kSemicolon});
			_operators.push_back(Parse::key_t{"?", Operators::kQuestion});
			_operators.push_back(Parse::key_t{"!", Operators::kExclemation});
			_operators.push_back(Parse::key_t{"+", Operators::kPlus});
			_operators.push_back(Parse::key_t{"-", Operators::kMinus});
			_operators.push_back(Parse::key_t{"*", Operators::kAsterisc});
			_operators.push_back(Parse::key_t{"/", Operators::kForwardSlash});
			_operators.push_back(Parse::key_t{"\\", Operators::kBackwardSlash});
			_operators.push_back(Parse::key_t{"%", Operators::kPercent});
			_operators.push_back(Parse::key_t{"&", Operators::kAmpersand});
			_operators.push_back(Parse::key_t{"@", Operators::kAt});
			_operators.push_back(Parse::key_t{"$", Operators::kDollar});
			_operators.push_back(Parse::key_t{"#", Operators::kHashtag});
			_operators.push_back(Parse::key_t{"^", Operators::kHat});
			_operators.push_back(Parse::key_t{"=", Operators::kIs});
			_operators.push_back(Parse::key_t{"|", Operators::kOr});
			_operators.push_back(Parse::key_t{"<", Operators::kLess});
			_operators.push_back(Parse::key_t{">", Operators::kMore});
			_operators.push_back(Parse::key_t{"==", Operators::kCompareEquals});
			_operators.push_back(Parse::key_t{"!=", Operators::kCompareDiffers});
			_operators.push_back(Parse::key_t{"<=", Operators::kLessOrEqual});
			_operators.push_back(Parse::key_t{">=", Operators::kMoreOrEqual});

			sEnumerator_for_operators = Utils::RangeEnumerator<Parse::key_t const&, std::vector<Parse::key_t>::const_iterator>(_operators.cbegin(), _operators.cend());

			_keywords.push_back(Parse::key_t{"bool", Keywords::kBool});
			_keywords.push_back(Parse::key_t{"int", Keywords::kInteger});
			_keywords.push_back(Parse::key_t{"real", Keywords::kReal});
			_keywords.push_back(Parse::key_t{"word", Keywords::kWord});
			_keywords.push_back(Parse::key_t{"type", Keywords::kStruct});

			sEnumerator_for_keywords = Utils::RangeEnumerator<Parse::key_t const&, std::vector<Parse::key_t>::const_iterator>(_keywords.cbegin(), _keywords.cend());
		}

	private:
		std::vector<Parse::key_t> _operators;
		std::vector<Parse::key_t> _keywords;
	};
	const InitEnumberators initEnumerators;
}

Compiler::ModulePtr SRBT::Parser::readModule(Parse::File const &file)
{
	Compiler::ModulePtr res;

	std::unique_ptr<Parse::SequenceNode> sequence;
	try
	{
		sequence = Parse::parse(file, sEnumerator_for_operators, sEnumerator_for_keywords);
	}
	catch(Parse::exception_unterminated_string const &e)
	{
		throw ParseException(file.path(), file.start() + e._line, "missing end \"");
	}
	catch(Parse::exception_string_terminated_by_backslash const &e)
	{
		throw ParseException(file.path(), file.start() + e._line, "ends with a \\");
	}
	catch(Parse::exception_special_character_not_supported const &e)
	{
		throw ParseException(file.path(), file.start() + e._line, std::string() + "special character \\" + e._character + "not supported");
	}

	return res;
}
