#pragma once

#include "Properties.hpp"

#include "Utils/Macros.hpp"
#include "Utils/Sharedpointer.hpp"

#include <map>

#include <experimental/filesystem>

namespace SRBT
{
namespace Interpret
{
namespace FR
{
	class Origin;
	class FileOrigin;
	class StructOrigin;
	using OriginPtr = std::shared_ptr<Origin>;
	using FileOriginPtr = std::shared_ptr<FileOrigin>;
	using StructOriginPtr = std::shared_ptr<StructOrigin>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	using path_t = std::experimental::filesystem::path;

	class Origin
	{
	public:
		virtual ~Origin() = default;

		/*abstract*/
		virtual bool operator==(Origin const &) const = 0;
		virtual std::string toString() const = 0;
		virtual FileOrigin const & fileOrigin() const = 0;
	};

	class FileOrigin final : public Origin
	{
	public:
		explicit FileOrigin(path_t const & inPath) : _path(inPath) {}
		~FileOrigin() override = default;

		bool operator==(FileOrigin const &) const;

		path_t const & path() const { return _path; }

		/* override from Origin */
		bool operator==(Origin const &) const override;
		std::string toString() const override { return _path.u8string(); }
		FileOrigin const & fileOrigin() const override { return *this; }

	private:
		path_t _path;
	};

	class StructOrigin final : public Origin
	{
	public:
		StructOrigin(FileOriginPtr const &fileOrigin, const int line) : _fileOrigin(fileOrigin), _line(line) {}

		bool operator==(StructOrigin const &) const;

		bool operator==(Origin const &) const override;
		std::string toString() const override;
		FileOrigin const & fileOrigin() const override { return *_fileOrigin; }

	private:
		FileOriginPtr _fileOrigin;
		int _line;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Module
	{
	public:
		using Properties = std::map<std::string, CompleteType>;
		using id_t = std::underlying_type_t<PrimitiveType>;

	public:
		explicit Module(OriginPtr inOrigin) : _origin(std::move(inOrigin)), _moduleId(++_sModuleID) {}

		Origin const & origin() const { return *_origin; }

		id_t moduleId() const { return _moduleId; }

		Properties &properties() { return _properties; }

	private:
		OriginPtr _origin;
		Properties _properties;
		id_t _moduleId;

		static id_t _sModuleID;
	};

	using ModulePtr= std::shared_ptr<Module>;
}
}
}
