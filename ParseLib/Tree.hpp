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
		TokenNode(std::string &&token, const int line) : _token(std::move(token)), _line(line) {}

		std::string const &token() const { return _token; }
		int line() const { return _line; }

		TokenNode const * tryAsToken() const override { return this; }
		TokenNode * tryAsToken() override { return this; }

	private:
		std::string _token;
		int _line;
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

	std::unique_ptr<SequenceNode> parse(File const&, int line, Utils::Enumerator<std::string const&> &operators);
}
}
