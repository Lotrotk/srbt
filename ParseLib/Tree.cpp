#include "Tree.hpp"

#include "Utils/Exception.hpp"

using namespace SRBT;
using namespace SRBT::Parse;

#include <map>

namespace
{
	using operators_t = Utils::Enumerator<std::string const&>;

	enum struct return_code
	{
		kEOF,
		kToken,
		kNewLine,
	};

	return_code next(char const * &ioData, char const *const end, size_t &outLength, operators_t &operators)
	{
		return_code res = return_code::kEOF;
		outLength = 0;

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
				size_t const b = s.find(operators.current());
				if(b != std::string::npos)
				{
					if(b == 0)
					{
						outLength = operators.current().size();
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

class TreeCache::RealTreeCache final : public TreeCache
{
public:
	using cache_t = std::map<std::string, TreeNodePtr>;

public:
	cache_t _cache{};
	TreeNodePtr const _newline{new TokenNode("\n")};
};

std::pair<std::unique_ptr<SequenceNode>, std::unique_ptr<TreeCache>>SRBT::Parse::parse(File const &file, operators_t &operators, std::unique_ptr<TreeCache> inCache)
{
	std::unique_ptr<SequenceNode> res;

	const char *data = file.data();
	size_t const fileLength = file.length();
	if(!data || fileLength == 0)
	{
		return std::make_pair(std::move(res), std::move(inCache));
	}
	const char *const end = data + fileLength;

	res.reset(new SequenceNode);
	SequenceNode::list_t &list = res->list();

	if(!inCache)
	{
		inCache.reset(new TreeCache::RealTreeCache);
	}
	TreeCache::RealTreeCache &cache = static_cast<TreeCache::RealTreeCache&>(*inCache);

	while(true)
	{
		size_t length;
		return_code const code = next(data, end, length, operators);
		switch (code)
		{
		case return_code::kToken:
		{
			std::string token = std::string(data, length);
			TreeNodePtr &c = cache._cache[token];
			if(!c)
			{
				c.reset(new TokenNode(std::move(token)));
			}
			list.push_back(c);
			break;
		}
		case return_code::kNewLine:
			list.push_back(cache._newline);
			break;
		default:
			return std::make_pair(std::move(res), std::move(inCache));
		}

		data += length;
	}

	throw Utils::TechnicalException(std::string(__FILE__) + ": error in parse");
}
