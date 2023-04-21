#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "memory.hpp"

namespace vtk
{
	class LogicalDevice;

	class CommandPool
	{
		friend class CommandPoolBuilder;
	public:
		~CommandPool() noexcept;

		CommandPool(const CommandPool&) = delete;
		CommandPool& operator=(const CommandPool&) = delete;
		CommandPool(CommandPool&&) = delete;
		CommandPool& operator=(CommandPool&&) = delete;

		VkCommandBuffer create();
		std::vector<VkCommandBuffer> create(int count);
	private:
		explicit CommandPool(const CommandPoolBuilder& builder);

		Ref<LogicalDevice> mDevice;
		VkCommandPool mHandle;
	};

	class CommandPoolBuilder
	{
		friend class CommandPool;
	public:
		explicit CommandPoolBuilder(Ref<LogicalDevice> device) noexcept;

		CommandPoolBuilder& index(uint32_t index) noexcept;

		Ref<CommandPool> build() const;
	private:
		Ref<LogicalDevice> mDevice;
		uint32_t mIndex = 0;
	};
}    
