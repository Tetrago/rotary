#pragma once

#include <vulkan/vulkan.h>

#include "logical_device.hpp"
#include "memory.hpp"

namespace vtk
{
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
