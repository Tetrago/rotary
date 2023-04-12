#include "vtk/logical_device.hpp"

#include <stdexcept>
#include <iterator>
#include <ranges>
#include <algorithm>
#include <diag/diag.hpp>

namespace vtk
{
	std::optional<uint32_t> find_queue_family(std::span<VkQueueFamilyProperties const> queueFamilies, QueueType target) noexcept
	{
		for(uint32_t i = 0; i < queueFamilies.size(); ++i)
		{
			const VkQueueFamilyProperties& props = queueFamilies[i];

			switch(target)
			{
			case QueueType::Graphics:
				if(props.queueFlags & VK_QUEUE_GRAPHICS_BIT) return i;
			}
		}

		return std::nullopt;
	}

	LogicalDevice::LogicalDevice(const LogicalDeviceBuilder& builder)
	{
		float const priority = 1.0f;
		
		std::vector<VkDeviceQueueCreateInfo> queueInfos;
		std::ranges::transform(builder.mQueues, std::back_inserter(queueInfos), [&priority](const std::pair<QueueType, uint32_t>& pair)
		{
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.queueFamilyIndex = pair.second;
			createInfo.queueCount = 1;
			createInfo.pQueuePriorities = &priority;

			return createInfo;
		});

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
		if(auto opt = find_queue_family(mDevice.queueFamilies, QueueType::Graphics))
		{
			mQueues[QueueType::Graphics] = opt.value();
		}
		else
		{
			diag::logger("vtk").warn("Failed to find available graphics queue in selected physical device `{}`", mDevice.properties.deviceName);
		}

		return *this;
	}

	LogicalDevice LogicalDeviceBuilder::build() const
	{
		return LogicalDevice(*this);
	}
}    
