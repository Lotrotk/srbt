#include "Interpreter.hpp"

#include "Exception.hpp"
#include "Keys.hpp"
#include "Statement.hpp"

#include "Tokenize/Tree.hpp"

#include "Utils/Exception.hpp"

using namespace SRBT;
using namespace SRBT::Interpret;

namespace
{
	Utils::RangeEnumerator<Tokenize::key_t const&, std::vector<Tokenize::key_t>::const_iterator> sEnumerator_for_operators;
	Utils::RangeEnumerator<Tokenize::key_t const&, std::vector<Tokenize::key_t>::const_iterator> sEnumerator_for_keywords;

	struct InitEnumberators
	{
		InitEnumberators()
		{
			_operators.push_back(Tokenize::key_t{"(", Operators::kBraceLeft});
			_operators.push_back(Tokenize::key_t{")", Operators::kBraceRight});
			_operators.push_back(Tokenize::key_t{"{", Operators::kCurlyBraceLeft});
			_operators.push_back(Tokenize::key_t{"}", Operators::kCurlyBraceRight});
			_operators.push_back(Tokenize::key_t{"[", Operators::kSquareBraceLeft});
			_operators.push_back(Tokenize::key_t{"]", Operators::kSquareBraceRight});
			_operators.push_back(Tokenize::key_t{".", Operators::kPoint});
			_operators.push_back(Tokenize::key_t{",", Operators::kComma});
			_operators.push_back(Tokenize::key_t{":", Operators::kColon});
			_operators.push_back(Tokenize::key_t{";", Operators::kSemicolon});
			_operators.push_back(Tokenize::key_t{"?", Operators::kQuestion});
			_operators.push_back(Tokenize::key_t{"!", Operators::kExclemation});
			_operators.push_back(Tokenize::key_t{"+", Operators::kPlus});
			_operators.push_back(Tokenize::key_t{"-", Operators::kMinus});
			_operators.push_back(Tokenize::key_t{"*", Operators::kAsterisc});
			_operators.push_back(Tokenize::key_t{"/", Operators::kForwardSlash});
			_operators.push_back(Tokenize::key_t{"\\", Operators::kBackwardSlash});
			_operators.push_back(Tokenize::key_t{"%", Operators::kPercent});
			_operators.push_back(Tokenize::key_t{"&", Operators::kAmpersand});
			_operators.push_back(Tokenize::key_t{"@", Operators::kAt});
			_operators.push_back(Tokenize::key_t{"$", Operators::kDollar});
			_operators.push_back(Tokenize::key_t{"#", Operators::kHashtag});
			_operators.push_back(Tokenize::key_t{"^", Operators::kHat});
			_operators.push_back(Tokenize::key_t{"=", Operators::kIs});
			_operators.push_back(Tokenize::key_t{"|", Operators::kOr});
			_operators.push_back(Tokenize::key_t{"<", Operators::kLess});
			_operators.push_back(Tokenize::key_t{">", Operators::kMore});
			_operators.push_back(Tokenize::key_t{"==", Operators::kCompareEquals});
			_operators.push_back(Tokenize::key_t{"!=", Operators::kCompareDiffers});
			_operators.push_back(Tokenize::key_t{"<=", Operators::kLessOrEqual});
			_operators.push_back(Tokenize::key_t{">=", Operators::kMoreOrEqual});
			_operators.push_back(Tokenize::key_t{">=", Operators::kHashHashtag});

			sEnumerator_for_operators = Utils::RangeEnumerator<Tokenize::key_t const&, std::vector<Tokenize::key_t>::const_iterator>(_operators.cbegin(), _operators.cend());

			_keywords.push_back(Tokenize::key_t{"bool", Keywords::kBool});
			_keywords.push_back(Tokenize::key_t{"int", Keywords::kInteger});
			_keywords.push_back(Tokenize::key_t{"real", Keywords::kReal});
			_keywords.push_back(Tokenize::key_t{"word", Keywords::kWord});
			_keywords.push_back(Tokenize::key_t{"type", Keywords::kStruct});

			sEnumerator_for_keywords = Utils::RangeEnumerator<Tokenize::key_t const&, std::vector<Tokenize::key_t>::const_iterator>(_keywords.cbegin(), _keywords.cend());
		}

	private:
		std::vector<Tokenize::key_t> _operators;
		std::vector<Tokenize::key_t> _keywords;
	};
	const InitEnumberators initEnumerators;

	void interPretStatements(Compiler::Module &module, Tokenize::SequenceNode &sequence);
}

Compiler::ModulePtr SRBT::Interpret::interpretModule(Tokenize::File const &file)
{
	Compiler::ModulePtr res;

	std::unique_ptr<Tokenize::SequenceNode> sequence;
	try
	{
		sequence = Tokenize::parse(file, sEnumerator_for_operators, sEnumerator_for_keywords);
	}
	catch(Tokenize::exception_unterminated_string const &e)
	{
		throw ParseException(file.path(), file.start() + e._line, "missing end \"");
	}
	catch(Tokenize::exception_string_terminated_by_backslash const &e)
	{
		throw ParseException(file.path(), file.start() + e._line, "ends with a \\");
	}
	catch(Tokenize::exception_special_character_not_supported const &e)
	{
		throw ParseException(file.path(), file.start() + e._line, std::string() + "special character \\" + e._character + "not supported");
	}

	Compiler::OriginPtr origin;
	if(Tokenize::TextFile const *const textFile = dynamic_cast<Tokenize::TextFile const*>(&file))
	{
		origin.reset(new Compiler::FileOrigin(textFile->path()));
	}
	else
	{
		throw Utils::TechnicalException("only textfile origins defined");
	}

	res.reset(new Compiler::Module(origin));

	interPretStatements(*res, *sequence);

	return res;
}

namespace
{
	void interPretStatements(Compiler::Module &module, Tokenize::SequenceNode &sequence)
	{
		for(Tokenize::iterator_t it = sequence.list().begin(); it != sequence.list().end();)
		{
			Tokenize::TreeNode &head = **it;
			if(Tokenize::KeyNode const *const key = head.tryAsKeyNode())
			{
				switch(key->key())
				{
				case kHashtag:
					++it;
					if(it == sequence.list().end())
					{
						throw ParseException(module.origin().fileOrigin().path(), key->line(), "incomplete statement");
					}
					interpretMember(module, sequence, it);
					break;
				default:
					throw ParseException(module.origin().fileOrigin().path(), key->line(), "keyword invalid here");
				}
			}
		}
	}
}
