#include "vtk/instance.hpp"

#include <algorithm>
#include <ranges>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <diag/diag.hpp>

namespace vtk
{
	namespace
	{
		std::optional<std::vector<std::string_view>> get_missing_extensions(std::span<VkExtensionProperties const> extensions, std::span<std::string_view const> names) noexcept
		{
			std::vector<std::string_view> missing(names.begin(), names.end());

			// Remove any extension names that we can find in our list
			// This leaves any missing extensions behind
			missing.erase(std::remove_if(missing.begin(), missing.end(), [&extensions](std::string_view name)
			{
				// If there are no matching extensions in the list, then we want to keep the current name in the missing list
				return !std::ranges::filter_view(extensions, [&name](const VkExtensionProperties& props){ return props.extensionName == name; }).empty();
			}), missing.end());

			// Return the missing extensions, or an empty optional
			return missing.empty() ? std::nullopt : std::optional(missing);
		}
	}

	Instance::Instance(VkInstance handle) noexcept
		: mHandle(handle)
	{}

	Instance::~Instance() noexcept
	{
		vkDestroyInstance(mHandle, nullptr);
	}

	Instance::Instance(Instance&& other) noexcept
		: mHandle(other.mHandle)
	{
		other.mHandle = VK_NULL_HANDLE;
	}

	InstanceBuilder::InstanceBuilder() noexcept
	{
		mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	}

	InstanceBuilder& InstanceBuilder::application(std::string_view name, int major, int minor, int patch) noexcept
	{
		mAppInfo.pApplicationName = name.data();
		mAppInfo.applicationVersion = VK_MAKE_VERSION(major, minor, patch);

		return *this;
	}

	InstanceBuilder& InstanceBuilder::engine(std::string_view name, int major, int minor, int patch) noexcept
	{
		mAppInfo.pEngineName = name.data();
		mAppInfo.engineVersion = VK_MAKE_VERSION(major, minor, patch);

		return *this;
	}

	InstanceBuilder& InstanceBuilder::extensions(std::span<std::string_view const> names) noexcept
	{
		std::ranges::transform(names, std::back_inserter(mExtensionNames), [](std::string_view name){ return name.data(); });

		return *this;
	}

	Instance InstanceBuilder::build() const
	{
		if(auto missing = get_missing_extensions(get_instance_extensions(), mExtensionNames))
		{
			std::ranges::for_each(missing.value(), [](std::string_view name)
			{
				diag::logger("vtk").error("Missing instance extension `{}`", name);
			});

			throw std::runtime_error("Not all required instance extensions are present");
		}

		std::vector<const char*> extensions;
		std::ranges::transform(mExtensionNames, std::back_inserter(extensions), [](std::string_view name){ return name.data(); });

		std::vector<const char*> layers;
		std::ranges::transform(mLayerNames, std::back_inserter(layers), [](std::string_view name){ return name.data(); });

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &mAppInfo;
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledLayerCount = layers.size();
		createInfo.ppEnabledLayerNames = layers.data();

		VkInstance handle;
		if(vkCreateInstance(&createInfo, nullptr, &handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan instance");
		}

		return Instance(handle);
	}

	std::vector<VkExtensionProperties> get_instance_extensions() noexcept
	{
		uint32_t count;
		vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
		std::vector<VkExtensionProperties> extensions(count);
		vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

		return extensions;
	}
}    
