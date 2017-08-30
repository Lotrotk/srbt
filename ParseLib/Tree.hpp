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
	class KeyNode;

	class TreeNode
	{
	public:
		virtual ~TreeNode() = default;

		virtual TokenNode * tryAsToken() { return nullptr; }
		virtual SequenceNode * tryAsSequence() { return nullptr; }
		virtual KeyNode * tryAsKeyNode() { return nullptr; }
	};
	MAKE_SHARED_PTR(TreeNode);

	class TokenNode final : public TreeNode
	{
	public:
		TokenNode(std::string &&token, int const line) : _token(std::move(token)), _line(line) {}

		std::string const &token() const { return _token; }
		int line() const { return _line; }

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

		SequenceNode * tryAsSequence() override { return this; }

	private:
		list_t _list;
	};

	class KeyNode final : public TreeNode
	{
	public:
		KeyNode(int const key, int const line) : _key(key), _line(line) {}

		int key() const { return _key; }
		int line() const { return _line; }

		KeyNode * tryAsKeyNode() override { return this; }

	private:
		int _key;
		int _line;
	};

	using key_t = std::pair<std::string, int>;

	/**
	 * @param line : at which the file begins
	 * @param operators : operators are split even if no delimiters between (e.g. 4+5 -> '4','+','5')
	 * @param keywords : special tokens
	 * @return a sequence of tokennodes and keynodes
	 */
	std::unique_ptr<SequenceNode> parse(File const&, int line, Utils::Enumerator<key_t const&> &operators, Utils::Enumerator<key_t const&> &keywords);
}
}
