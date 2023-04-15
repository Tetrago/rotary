#include "vtk/logical_device.hpp"

#include <stdexcept>
#include <iterator>
#include <ranges>
#include <algorithm>
#include <diag/diag.hpp>

namespace vtk
{
	LogicalDevice::LogicalDevice(const LogicalDeviceBuilder& builder)
		: mInstance(builder.mInstance)
		, mPhysicalDevice(builder.mDevice)
	{
		std::unordered_map<QueueType, uint32_t> queueCreationIndices;
		std::vector<VkDeviceQueueCreateInfo> queueInfos;

		float const priority = 1.0f;
		for(auto pair : builder.mQueues)
		{
			const auto& [type, index] = pair;

			auto result = std::ranges::find_if(queueCreationIndices, [&pair](const std::pair<QueueType, uint32_t>& p){ return p.second == pair.second; });
			if(result != queueCreationIndices.end())
			{
				queueCreationIndices[type] = result->second;
				continue;
			}
			else
			{
				queueCreationIndices[type] = index;
			}

			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.queueFamilyIndex = index;
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &priority;

			queueInfos.push_back(createInfo);
		};

		std::vector<const char*> extensions;
		extensions.reserve(builder.mExtensionNames.size());
		std::ranges::transform(builder.mExtensionNames, std::back_inserter(extensions), [](std::string_view name){ return name.data(); });
		extensions.erase(std::get<0>(std::ranges::unique(extensions)), extensions.end());

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueInfos.data();
		createInfo.queueCreateInfoCount = queueInfos.size();
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkPhysicalDeviceFeatures features{};
		createInfo.pEnabledFeatures = &features;

		if(vkCreateDevice(mPhysicalDevice.handle, &createInfo, nullptr, &mHandle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan logical device");
		}

		for(const auto& [type, index] : queueCreationIndices)
		{
			mQueues[type].first = index;
			vkGetDeviceQueue(mHandle, index, 0, &mQueues[type].second);
		}
	}

	LogicalDevice::~LogicalDevice() noexcept
	{
		if(mHandle == VK_NULL_HANDLE) return;
		vkDestroyDevice(mHandle, nullptr);
	}
	
	LogicalDevice::LogicalDevice(LogicalDevice&& other) noexcept
		: mInstance(std::move(other.mInstance))
		, mHandle(other.mHandle)
		, mPhysicalDevice(std::move(other.mPhysicalDevice))
		, mQueues(std::move(other.mQueues))
	{
		other.mHandle = VK_NULL_HANDLE;
	}

	LogicalDeviceBuilder::LogicalDeviceBuilder(Ref<Instance> instance, const PhysicalDevice& device) noexcept
		: mInstance(std::move(instance))
		, mDevice(device)
	{}

	LogicalDeviceBuilder& LogicalDeviceBuilder::addGraphicsQueue() noexcept
	{
		for(uint32_t i = 0; i < mDevice.queueFamilies.size(); ++i)
		{
			if(!(mDevice.queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) continue;

			mQueues[QueueType::Graphics] = i;
			return *this;
		}

		diag::logger("vtk").warn("Failed to find available graphics queue in selected physical device `{}`", mDevice.properties.deviceName);
		return *this;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::addPresentQueue(VkSurfaceKHR surface) noexcept
	{
		for(uint32_t i = 0; i < mDevice.queueFamilies.size(); ++i)
		{
			VkBool32 support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(mDevice.handle, i, surface, &support);

			if(!support) continue;
			
			mQueues[QueueType::Present] = i;
			return *this;
		}

		diag::logger("vtk").warn("Failed to find available present queue in selected physical device `{}`", mDevice.properties.deviceName);
		return *this;
	}

	LogicalDeviceBuilder& LogicalDeviceBuilder::addSwapchainSupport() noexcept
	{
		mExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		return *this;
	}

	Ref<LogicalDevice> LogicalDeviceBuilder::build() const
	{
		return Ref<LogicalDevice>(new LogicalDevice(*this));
	}
}    
