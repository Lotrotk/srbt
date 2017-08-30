#include "Tree.hpp"

#include "Utils/Exception.hpp"

using namespace SRBT;
using namespace SRBT::Parse;

#include <map>

namespace
{
	using Parse::key_t;

	enum struct return_code
	{
		kEOF,
		kToken,
		kNewLine,
	};

	return_code next(char const * &ioData, char const *const end, size_t &outLength, int &outLinesSkipped, Utils::Enumerator<key_t const&> &operators)
	{
		return_code res = return_code::kEOF;
		outLength = 0;
		outLinesSkipped = 0;

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

std::unique_ptr<SequenceNode> SRBT::Parse::parse(File const &file, int line, Utils::Enumerator<key_t const&> &operators, Utils::Enumerator<key_t const&> &keywords)
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

	res.reset(new SequenceNode);
	SequenceNode::list_t &list = res->list();

	while(true)
	{
		size_t length;
		int linesSkipped;
		return_code const code = next(data, end, length, linesSkipped, operators);
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
		default:
			return res;
		}

		data += length;
	}

	throw Utils::TechnicalException(std::string(__FILE__) + ": error in parse");
}
