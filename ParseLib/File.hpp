#pragma once

#include <vector>

#include <experimental/filesystem>

namespace SRBT
{
namespace Parse
{
	using path_t = std::experimental::filesystem::path;

	class File
	{
	public:
		virtual ~File() = default;

		virtual char const *data() const = 0;
		virtual size_t length() const = 0;

		virtual path_t const& path() const = 0;
		virtual size_t start() const = 0;
	};

	class TextFile final : public File
	{
	public:
		bool open(path_t const &);

		char const *data() const override { return _buffer.data(); }
		size_t length() const override { return _buffer.size(); }

		path_t const& path() const override { return _path; }
		size_t start() const override { return 1; }

	private:
		path_t _path;
		std::vector<char> _buffer;
	};
}
}
