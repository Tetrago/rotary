#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "memory.hpp"

namespace vtk
{
	class LogicalDevice;

	class CommandPool
	{
	public:
		CommandPool(Ref<LogicalDevice> device, uint32_t index);
		~CommandPool() noexcept;

		CommandPool(const CommandPool&) = delete;
		CommandPool& operator=(const CommandPool&) = delete;
		CommandPool(CommandPool&& other) noexcept;
		CommandPool& operator=(CommandPool&&) = delete;

		VkCommandBuffer create();
		std::vector<VkCommandBuffer> create(int count);
	private:
		Ref<LogicalDevice> mDevice;
		VkCommandPool mHandle = VK_NULL_HANDLE;
	};
}    
