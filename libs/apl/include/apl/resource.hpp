#pragma once

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "manifest.hpp"

namespace apl
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	class Resource
	{
	public:
		virtual ~Resource() noexcept = default;

		const ResourceLocation& location() const noexcept { return mLocation; }
	protected:
		explicit Resource(const ResourceLocation& location) noexcept;

		ResourceLocation mLocation;
	};

	template<typename T>
	inline bool is_resource(const Ref<Resource>& resource)
	{
		return dynamic_cast<const T*>(resource.get());
	}

	template<typename T>
	inline Ref<T> get_resource(const Ref<Resource>& resource)
	{
		return std::dynamic_pointer_cast<T>(resource);
	}

	class TextResource : public Resource
	{
	public:
		TextResource(const ResourceLocation& location, std::string content) noexcept;

		const std::string& content() const noexcept { return mContent; }
	private:
		std::string mContent;
	};

	class BinaryResource : public Resource
	{
	public:
		BinaryResource(const ResourceLocation location, std::vector<uint8_t> bytes) noexcept;

		const std::vector<uint8_t> bytes() const noexcept { return mBytes; }
	private:
		std::vector<uint8_t> mBytes;
	};
}
