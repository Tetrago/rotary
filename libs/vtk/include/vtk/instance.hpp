#pragma once

#include <vector>
#include <span>
#include <string_view>
#include <vulkan/vulkan.h>

#include "memory.hpp"

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

		template<typename T>
		T get(std::string_view name)
		{
			return reinterpret_cast<T>(vkGetInstanceProcAddr(mHandle, name.data()));
		}
	private:
		explicit Instance(const InstanceBuilder& builder);

		VkInstance mHandle = VK_NULL_HANDLE;
		VkDebugUtilsMessengerEXT mMessenger = VK_NULL_HANDLE;
	};

	class InstanceBuilder
	{
		friend class Instance;
	public:
		InstanceBuilder() noexcept;

		InstanceBuilder& application(std::string_view name, int major, int minor, int patch) noexcept;
		InstanceBuilder& engine(std::string_view name, int major, int minor, int patch) noexcept;
		InstanceBuilder& extensions(std::span<std::string_view const> names) noexcept;
		InstanceBuilder& debug() noexcept;

		Ref<Instance> build() const;
	private:
		VkApplicationInfo mAppInfo{};
		std::vector<std::string_view> mExtensionNames;
		std::vector<std::string_view> mLayerNames;
		bool mDebug = false;
	};

	std::vector<VkExtensionProperties> get_instance_extensions() noexcept;
	std::vector<VkLayerProperties> get_instance_layers() noexcept;
}
