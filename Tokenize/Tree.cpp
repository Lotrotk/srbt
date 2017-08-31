#include "Tree.hpp"

#include "Utils/Exception.hpp"

using namespace SRBT;
using namespace SRBT::Tokenize;

#include <map>

namespace
{
	using Tokenize::key_t;

	enum struct return_code
	{
		kEOF,
		kToken,
		kNewLine,
		kString,
	};

	return_code next(char const * &ioData, char const *const end, size_t &outLength, int &outLinesSkipped, int &outLinesSkippedInString, Utils::Enumerator<key_t const&> &operators)
	{
		return_code res = return_code::kEOF;
		outLength = 0;
		outLinesSkipped = 0;
		outLinesSkippedInString = 0;

		if(ioData == end)
		{
		}
		else if(ioData[0] == ' ' || ioData[0] == '\t')
		{
			while((ioData[0] == ' ' || ioData[0] == '\t') && ioData != end)
			{
				++ioData;
			}

			if(ioData == end)
			{
			}
			else if(ioData[0] == '\n')
			{
				res = return_code::kNewLine;
			}
			else
			{
				res = return_code::kToken;
			}
		}
		else if(ioData[0] == '\n')
		{
			while((ioData[0] == ' ' || ioData[0] == '\t' || ioData[0] == '\n') && ioData != end)
			{
				++ioData;
				outLinesSkipped += ioData[0] == '\n';
			}

			if(ioData == end)
			{
			}
			else
			{
				res = return_code::kToken;
			}
		}
		else if(ioData[0] == '"')
		{
			char const *data = ioData;
			++ioData;
			while(true)
			{
				++data;
				if(data == end)
				{
					throw exception_unterminated_string();
				}
				else if(data[0] == '\n')
				{
					++outLinesSkippedInString;
				}
				else if(data[0] == '\\')
				{
					++data;
					if(data == end)
					{
						throw exception_unterminated_string();
					}
					if(!(data[0] == '\\' || data[0] == 'n' || data[0] == '"'))
					{
						exception_special_character_not_supported e;
						e._character = data[0];
						throw e;
					}
				}
				else if(data[0] == '"')
				{
					outLength = std::distance(ioData, data);
					return return_code::kString;
				}
			}
		}
		else
		{
			while(!(ioData[0] == ' ' || ioData[0] == '\t' || ioData[0] == '\n') && ioData != end)
			{
				++ioData;
				outLinesSkipped += ioData[0] == '\n';
			}

			if(ioData == end)
			{
			}
			else if(ioData[0] == '\n')
			{
				res = return_code::kNewLine;
			}
			else
			{
				res = return_code::kToken;
			}
		}

		if(res == return_code::kNewLine)
		{
			outLength = 1;
		}
		else if(res == return_code::kToken)
		{
			char const *d = ioData + 1;
			for(; !(d[0] == ' ' || d[0] == '\t' || d[0] == '\n') && d != end; ++d);
			outLength = std::distance(ioData, d);

			operators.reset();
			std::string const s = std::string(ioData, outLength);
			while(operators.next())
			{
				size_t const b = s.find(operators.current().first);
				if(b != std::string::npos)
				{
					if(b == 0)
					{
						outLength = operators.current().first.size();
					}
					else
					{
						outLength = b;
					}
					break;
				}
			}
		}

		return res;
	}
}

std::unique_ptr<SequenceNode> SRBT::Tokenize::parse(File const &file, Utils::Enumerator<key_t const&> &operators, Utils::Enumerator<key_t const&> &keywords)
{
	// check if operators from smallest to largest length
	{
		operators.reset();
		if(operators.next()) {
			size_t l = operators.current().first.length();
			if(l == 0)
			{
				throw Utils::TechnicalException("parse : operators must have length");
			}
			while(operators.next())
			{
				size_t ll = operators.current().first.length();
				if(ll < l)
				{
					throw Utils::TechnicalException("parse : operators must be sorted from smallest to largest");
				}
				l = ll;
			}
		}
	}

	std::unique_ptr<SequenceNode> res;

	const char *data = file.data();
	size_t const fileLength = file.length();
	if(!data || fileLength == 0)
	{
		return res;
	}
	const char *const end = data + fileLength;

	res.reset(new SequenceNode(file.start()));
	SequenceNode::list_t &list = res->list();

	int line = file.start();

	while(true)
	{
		size_t length;
		int linesSkipped;
		int linesSkippedInString;
		return_code code;
		try
		{
			code = next(data, end, length, linesSkipped, linesSkippedInString, operators);
		}
		catch(exception_unterminated_string &e)
		{
			e._line = line + linesSkipped;
			throw;
		}
		catch(exception_special_character_not_supported &e)
		{
			e._line = line + linesSkipped + linesSkippedInString;
			throw;
		}

		line += linesSkipped;
		switch (code)
		{
		case return_code::kToken:
		{
			std::string token = std::string(data, length);
			operators.reset();
			while(operators.next())
			{
				if(operators.current().first == token)
				{
					list.emplace_back(new KeyNode(operators.current().second, line));
					break;
				}
			}
			keywords.reset();
			while(keywords.next())
			{
				if(keywords.current().first == token)
				{
					list.emplace_back(new KeyNode(keywords.current().second, line));
					break;
				}
			}
			list.emplace_back(new TokenNode(std::move(token), line));
			break;
		}
		case return_code::kNewLine:
			list.emplace_back(new TokenNode("\n", line));
			break;
		case return_code::kString:
		{
			std::string s(data, length);
			for(std::string::iterator it = s.begin(); it != s.end(); ++it)
			{
				if(*it == '\\')
				{
					it = s.erase(it);
					if(it == s.end())
					{
						throw exception_string_terminated_by_backslash{line};
					}
					if(*it == 'n')
					{
						*it = '\n';
					}
				}
			}
			list.emplace_back(new StringNode(std::move(s), line));
			++data;
			line += linesSkippedInString;
			break;
		}
		default:
			return res;
		}

		data += length;
	}

	throw Utils::TechnicalException(std::string(__FILE__) + ": error in parse");
}
