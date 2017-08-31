#pragma once

#include "File.hpp"

#include "Utils/Enumerator.hpp"
#include "Utils/Sharedpointer.hpp"

#include "list"

namespace SRBT
{
namespace Tokenize
{
	class TokenNode;
	class SequenceNode;
	class KeyNode;
	class StringNode;

	class TreeNode
	{
	public:
		virtual ~TreeNode() = default;

		int line() const { return _line; }

		virtual TokenNode * tryAsToken() { return nullptr; }
		virtual StringNode * tryAsString() { return nullptr; }
		virtual SequenceNode * tryAsSequence() { return nullptr; }
		virtual KeyNode * tryAsKeyNode() { return nullptr; }

	protected:
		TreeNode(int const line) : _line(line) {}

	private:
		int _line;
	};
	MAKE_SHARED_PTR(TreeNode);

	class TokenNode final : public TreeNode
	{
	public:
		TokenNode(std::string &&token, int const line) : TreeNode(line), _token(std::move(token)) {}

		std::string const &token() const { return _token; }

		TokenNode * tryAsToken() override { return this; }

	private:
		std::string _token;
	};

	class StringNode final : public TreeNode
	{
	public:
		StringNode(std::string &&value, int const line) : TreeNode(line), _value(std::move(value)) {}

		std::string const &value() const { return _value; }

		StringNode * tryAsString() override { return this; }

	private:
		std::string _value;
	};

	class SequenceNode final : public TreeNode
	{
	public:
		using list_t = std::list<TreeNodePtr>;

	public:
		SequenceNode(int const line) : TreeNode(line) {}

		list_t &list() { return _list; }
		list_t const &list() const { return _list; }

		SequenceNode * tryAsSequence() override { return this; }

	private:
		list_t _list;
	};

	using iterator_t = SequenceNode::list_t::iterator;

	class KeyNode final : public TreeNode
	{
	public:
		KeyNode(int const key, int const line) : TreeNode(line), _key(key) {}

		int key() const { return _key; }

		KeyNode * tryAsKeyNode() override { return this; }

	private:
		int _key;
	};

	using key_t = std::pair<std::string, int>;

	struct exception_unterminated_string{ int _line; };
	struct exception_string_terminated_by_backslash{ int _line; };
	struct exception_special_character_not_supported{ int _line; char _character; };
	/**
	 * @brief parses file into sequence. Strings are defined by "...", special characters supported : '\\', '\n', '\"'
	 * @param operators : operators are split even if no delimiters between (e.g. 4+5 -> '4','+','5')
	 * @param keywords : special tokens
	 * @return a sequence of tokennodes, stringnodes and keynodes
	 * @throws exception_unterminated_string, exception_string_terminated_by_backslash, exception_special_character_not_supported
	 */
	std::unique_ptr<SequenceNode> parse(File const&, Utils::Enumerator<key_t const&> &operators, Utils::Enumerator<key_t const&> &keywords);
}
}
