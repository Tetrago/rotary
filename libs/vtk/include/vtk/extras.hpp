#pragma once

#include <stdexcept>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "logical_device.hpp"
#include "memory.hpp"

namespace vtk
{
	inline Holder<VkFence> create_fence(const LogicalDevice& device, bool signaled = false)
	{
		VkFenceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		if(signaled)
			createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkFence handle;
		if(vkCreateFence(device, &createInfo, nullptr, &handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create fence");
		}

		// LogicalDevice can only be allocated dynamically, so `this` can be safely captured
		const LogicalDevice* pDevice = &device;

		return Holder<VkFence>(handle, [pDevice](VkFence handle){ vkDestroyFence(*pDevice, handle, nullptr); });
	}

	inline Holder<VkSemaphore> create_semaphore(const LogicalDevice& device)
	{
		VkSemaphoreCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkSemaphore handle;
		if(vkCreateSemaphore(device, &createInfo, nullptr, &handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create semaphore");
		}

		// LogicalDevice can only be allocated dynamically, so `this` can be safely captured
		const LogicalDevice* pDevice = &device;

		return Holder<VkSemaphore>(handle, [pDevice](VkSemaphore handle){ vkDestroySemaphore(*pDevice, handle, nullptr); });
	}
	
	inline void wait(const LogicalDevice& device, VkFence fence) noexcept
	{
		vkWaitForFences(device, 1, &fence, true, std::numeric_limits<uint64_t>::max());
	}

	inline void reset(const LogicalDevice& device, VkFence fence) noexcept
	{
		vkResetFences(device, 1, &fence);
	}

	inline void reset(VkCommandBuffer buffer) noexcept
	{
		vkResetCommandBuffer(buffer, 0);
	}

	inline void begin(VkCommandBuffer buffer, bool single = false) noexcept
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if(single)
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(buffer, &beginInfo);
	}

	inline void end(VkCommandBuffer buffer) noexcept
	{
		vkEndCommandBuffer(buffer);
	}
}
