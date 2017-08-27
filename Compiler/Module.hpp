#pragma once

#include "MutableProperties.hpp"

#include "Utils/Macros.hpp"

#include <map>
#include <memory>

#include <experimental/filesystem>

using namespace std::experimental;

namespace SRBT
{
namespace Compiler
{
	class Origin;
	class FileOrigin;
	using OriginUPtr = std::unique_ptr<Origin>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Origin
	{
	public:
		virtual ~Origin() = default;

		/*abstract*/
		virtual bool operator==(Origin const &) const = 0;
		virtual std::string toString() const = 0;

		virtual FileOrigin const & fileOrigin() const = 0;
		virtual OriginUPtr clone() const = 0;
	};

	class FileOrigin : public Origin
	{
	public:
		explicit FileOrigin(filesystem::path const & inPath) : _path(inPath) {}
		~FileOrigin() override = default;

		bool operator==(FileOrigin const &) const;

		filesystem::path const & path() const { return _path; }

		/* override from Origin */
		bool operator==(Origin const &) const override;
		std::string toString() const override { return _path.u8string(); }
		FileOrigin const & fileOrigin() const override { return *this; }
		OriginUPtr clone() const override { return OriginUPtr(new FileOrigin(*this)); }
	private:
		filesystem::path _path;
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
			explicit Member(Property::Type const type) : _type(type) {}

			Property::Type type() const { return _type; }
			PropertyPtr const & property() const { return _property; }

		private:
			explicit Member(PropertyPtr const &);

		private:
			Property::Type _type;
			PropertyPtr _property;
		};

		using Properties = std::map<std::string, Member>;

	public:
		explicit Module(OriginUPtr inOrigin) : _origin(std::move(inOrigin)) {}

		Origin const & origin() const { return *_origin; }

	private:
		OriginUPtr _origin;
		Properties _properties;

	private:
		friend class MAccess;
	};

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
