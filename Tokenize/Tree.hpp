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
	class BracesNode;

	class TreeNode
	{
	public:
		virtual ~TreeNode() = default;

		int line() const { return _line; }

		virtual TokenNode * tryAsToken() { return nullptr; }
		virtual StringNode * tryAsString() { return nullptr; }
		virtual SequenceNode * tryAsSequence() { return nullptr; }
		virtual KeyNode * tryAsKeyNode() { return nullptr; }
		virtual BracesNode * tryAsBracesNode() { return nullptr; }

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

	using list_t = std::list<TreeNodePtr>;
	using iterator_t = list_t::iterator;

	class SequenceNode : public TreeNode
	{
	public:
		SequenceNode(int const line) : TreeNode(line) {}

		list_t &list() { return _list; }

		SequenceNode * tryAsSequence() override { return this; }

	private:
		list_t _list;
	};

	class KeyNode final : public TreeNode
	{
	public:
		KeyNode(int const key, int const line) : TreeNode(line), _key(key) {}

		int key() const { return _key; }

		KeyNode * tryAsKeyNode() override { return this; }

	private:
		int _key;
	};

	class BracesNode final : public SequenceNode
	{
	public:
		BracesNode(int const line, int leftBrace) : SequenceNode(line), _leftBrace(leftBrace) {}

		int leftBrace() const { return _leftBrace; }

		using TreeNode::tryAsKeyNode;
		BracesNode * tryAsBracesNode() override { return this; }

	private:
		int const _leftBrace;
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
	std::shared_ptr<SequenceNode> parse(File const&, Utils::Enumerator<key_t const&> &operators, Utils::Enumerator<key_t const&> &keywords);

	struct exception_unterminated_left_brace{ int _line; int _leftBrace; };
	struct exception_right_brace{ int _line; int _rightBrace; };
	/**
	 * @brief parseBraces
	 * @param path
	 * @param braces
	 * @return
	 * @throws exception_unterminated_left_brace, exception_right_brace
	 */
	void parseBraces(path_t const &path, std::shared_ptr<SequenceNode>&, Utils::Enumerator<std::pair<int, int> const &> &braces);
}
}
