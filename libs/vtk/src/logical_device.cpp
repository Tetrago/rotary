#include "vtk/logical_device.hpp"

#include <stdexcept>
#include <iterator>
#include <ranges>
#include <algorithm>
#include <diag/diag.hpp>

namespace vtk
{
	LogicalDevice::LogicalDevice(const LogicalDeviceBuilder& builder)
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

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueInfos.data();
		createInfo.queueCreateInfoCount = queueInfos.size();

		VkPhysicalDeviceFeatures features{};
		createInfo.pEnabledFeatures = &features;

		if(vkCreateDevice(builder.mDevice.handle, &createInfo, nullptr, &mHandle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan logical device");
		}

		for(const auto& [type, index] : queueCreationIndices)
		{
			vkGetDeviceQueue(mHandle, index, 0, &mQueues[type]);
		}
	}

	LogicalDevice::~LogicalDevice() noexcept
	{
		vkDestroyDevice(mHandle, nullptr);
	}
	
	LogicalDevice::LogicalDevice(LogicalDevice&& other) noexcept
		: mHandle(other.mHandle)
		, mPhysicalDevice(std::move(other.mPhysicalDevice))
	{
		other.mHandle = VK_NULL_HANDLE;
	}

	LogicalDeviceBuilder::LogicalDeviceBuilder(const PhysicalDevice& device) noexcept
		: mDevice(device)
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

	LogicalDevice LogicalDeviceBuilder::build() const
	{
		return LogicalDevice(*this);
	}
}    
