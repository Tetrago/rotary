#include "apl/resource.hpp"

namespace apl
{
	Resource::Resource(const ResourceLocation& location) noexcept
		: mLocation(location)
	{}

	TextResource::TextResource(const ResourceLocation& location, std::string content) noexcept
		: Resource(location)
		, mContent(std::move(content))
	{}

	BinaryResource::BinaryResource(const ResourceLocation& location, std::vector<uint8_t> bytes) noexcept
		: Resource(location)
		, mBytes(std::move(bytes))
	{}
}
