#pragma once

#include "File.hpp"

#include "Utils/Enumerator.hpp"
#include "Utils/Sharedpointer.hpp"

#include "list"

namespace SRBT
{
namespace Parse
{
	class TokenNode;
	class SequenceNode;

	class TreeNode
	{
	public:
		virtual ~TreeNode() = default;

		virtual TokenNode const * tryAsToken() const { return nullptr; }
		virtual TokenNode * tryAsToken() { return nullptr; }
		virtual SequenceNode const * tryAsSequence() const { return nullptr; }
		virtual SequenceNode * tryAsSequence() { return nullptr; }
	};
	MAKE_SHARED_PTR(TreeNode);

	class TokenNode final : public TreeNode
	{
	public:
		TokenNode(std::string &&token) : _token(std::move(token)) {}

		std::string const &token() const { return _token; }

		TokenNode const * tryAsToken() const override { return this; }
		TokenNode * tryAsToken() override { return this; }

	private:
		std::string _token;
	};

	class SequenceNode final : public TreeNode
	{
	public:
		using list_t = std::list<TreeNodePtr>;

	public:
		list_t &list() { return _list; }
		list_t const &list() const { return _list; }

		SequenceNode const * tryAsSequence() const override { return this; }
		SequenceNode * tryAsSequence() override { return this; }

	private:
		list_t _list;
	};

	class TreeCache
	{
	public:
		class RealTreeCache;

	public:
		virtual ~TreeCache() = default;
	private:
		TreeCache() = default;
	private:
		friend class RealTreeCache;
	};

	/**
	 * @brief parses the file
	 * @param operators : are recognized within tokens and treaded as a separate token (e.g. 4+4 -> '4', '+', '4')
	 * @param cache : is also returned, contains caching for tokens. Useful if multiple files with the same format are parsed, minimizes quantity of memory that is dynamically allocated to build the trees.
	 * @return both the sequence (if there was input) and the cache that can be used in a second call
	 */
	std::pair<std::unique_ptr<SequenceNode>, std::unique_ptr<TreeCache>> parse
	(
		File const&,
		Utils::Enumerator<std::string const&> &operators,
		std::unique_ptr<TreeCache> cache = std::unique_ptr<TreeCache>()
	);
}
}
