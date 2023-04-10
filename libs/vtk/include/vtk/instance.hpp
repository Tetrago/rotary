#pragma once

#include <vector>
#include <span>
#include <string_view>
#include <vulkan/vulkan.h>

namespace vtk
{
	class Instance
	{
		friend class InstanceBuilder;
	public:
		~Instance() noexcept;

		Instance(const Instance&) = delete;
		Instance& operator=(const Instance&) = delete;
		Instance(Instance&& other) noexcept;
		Instance& operator=(Instance&&) = delete;

		operator VkInstance() const noexcept { return mHandle; }
	private:
		explicit Instance(VkInstance handle) noexcept;

		VkInstance mHandle;
	};

	class InstanceBuilder
	{
		friend class Instance;
	public:
		InstanceBuilder() noexcept;

		InstanceBuilder& application(std::string_view name, int major, int minor, int patch) noexcept;
		InstanceBuilder& engine(std::string_view name, int major, int minor, int patch) noexcept;
		InstanceBuilder& extensions(std::span<const char* const> names) noexcept;

		Instance build() const;
	private:
		VkApplicationInfo mAppInfo{};
		std::vector<const char*> mExtensionNames;
	};

	std::vector<VkExtensionProperties> get_instance_extensions() noexcept;
}
