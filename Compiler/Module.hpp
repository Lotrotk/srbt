#pragma once

#include "MutableProperties.hpp"

#include "Utils/Macros.hpp"

#include <map>
#include <memory>

#include <experimental/filesystem>

namespace SRBT
{
namespace Compiler
{
	class Origin;
	class FileOrigin;
	class StructOrigin;
	MAKE_SHARED_PTR(Origin);
	MAKE_SHARED_PTR(FileOrigin);
	MAKE_SHARED_PTR(StructOrigin);

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
		class Member
		{
		public:
			explicit Member(BMPropertyPtr const & property);
			explicit Member(IMPropertyPtr const & property);
			explicit Member(RMPropertyPtr const & property);
			explicit Member(OMPropertyPtr const & property);
			explicit Member(WMPropertyPtr const & property);
			explicit Member(CompleteType const &type) : _type(type) {}

			CompleteType type() const { return _type; }
			PropertyPtr const & property() const { return _property; }

		private:
			explicit Member(PropertyPtr const &);

		private:
			CompleteType _type;
			PropertyPtr _property;
		};

		using Properties = std::map<std::string, Member>;
		using id_t = unsigned int;

	public:
		explicit Module(OriginPtr inOrigin) : _origin(std::move(inOrigin)), _moduleId(++_sModuleID), _object_type{id_t(_moduleId)}, _struct_type{PrimitiveType::kStruct, _object_type.front()} {}

		Origin const & origin() const { return *_origin; }

		id_t moduleId() const { return _moduleId; }
		CompleteType &objectType() const { return _object_type; }
		CompleteType &structType() const { return _struct_type; }

	private:
		OriginPtr _origin;
		Properties _properties;
		id_t _moduleId;
		CompleteType _object_type;
		CompleteType _struct_type;

		static id_t _sModuleID;

	private:
		friend class MAccess;
	};

	MAKE_SHARED_PTR(Module);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class MAccess
	{
	protected:
		static Module::Properties const & const_ioProperties(Module const & module) { return module._properties; }
		static Module::Properties & ioProperties(Module & module) { return module._properties; }

	private:
		STATIC_ALLOCATION_ONLY
	};
}
}
