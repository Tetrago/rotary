#include "vtk/instance.hpp"

#include <stdexcept>

namespace vtk
{
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

	Instance InstanceBuilder::build() const
	{
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &mAppInfo;

		VkInstance handle;
		if(vkCreateInstance(&createInfo, nullptr, &handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan instance");
		}

		return Instance(handle);
	}
}    
