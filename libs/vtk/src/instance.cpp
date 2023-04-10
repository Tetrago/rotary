#include "vtk/instance.hpp"

#include <optional>
#include <stdexcept>
#include <diag/diag.hpp>

namespace vtk
{
	namespace
	{
		std::optional<std::vector<const char*>> get_missing_extensions(std::span<VkExtensionProperties const> extensions, std::span<const char* const> names) noexcept
		{
			std::vector<const char*> missing;
			
			for(const char* name : names)
			{
				for(const VkExtensionProperties& props : extensions)
				{
					if(strcmp(name, props.extensionName) != 0) continue;
					goto found;
				}

				missing.push_back(name);
			found:;
			}

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

	InstanceBuilder& InstanceBuilder::extensions(std::span<const char* const> names) noexcept
	{
		mExtensionNames.insert(mExtensionNames.end(), names.begin(), names.end());

		return *this;
	}

	Instance InstanceBuilder::build() const
	{
		if(auto missing = get_missing_extensions(get_instance_extensions(), mExtensionNames))
		{
			for(const char* name : missing.value())
			{
				diag::Logger::logger("vtk").error("Missing instance extension `{}`", name);
			}

			throw std::runtime_error("Not all required instance extensions are present");
		}

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &mAppInfo;
		createInfo.enabledExtensionCount = mExtensionNames.size();
		createInfo.ppEnabledExtensionNames = mExtensionNames.data();

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
