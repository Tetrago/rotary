#include "vtk/command.hpp"

#include <stdexcept>

#include "vtk/logical_device.hpp"

namespace vtk
{
	CommandPool::CommandPool(const CommandPoolBuilder& builder)
		: mDevice(builder.mDevice)
	{
		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.queueFamilyIndex = builder.mIndex;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if(vkCreateCommandPool(*mDevice, &createInfo, nullptr, &mHandle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create command pool");
		}
	}

	CommandPool::~CommandPool() noexcept
	{
		if(mHandle == VK_NULL_HANDLE) return;
		vkDestroyCommandPool(*mDevice, mHandle, nullptr);
	}

	CommandPool::CommandPool(CommandPool&& other) noexcept
		: mDevice(std::move(other.mDevice))
		, mHandle(other.mHandle)
	{
		other.mHandle = VK_NULL_HANDLE;
	}

	VkCommandBuffer CommandPool::create()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mHandle;
		allocInfo.commandBufferCount = 1;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		VkCommandBuffer handle;
		if(vkAllocateCommandBuffers(*mDevice, &allocInfo, &handle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers");
		}

		return handle;
	}

	std::vector<VkCommandBuffer> CommandPool::create(int count)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mHandle;
		allocInfo.commandBufferCount = count;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		std::vector<VkCommandBuffer> buffers(count);
		if(vkAllocateCommandBuffers(*mDevice, &allocInfo, buffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers");
		}

		return buffers;
	}

	CommandPoolBuilder::CommandPoolBuilder(Ref<LogicalDevice> device) noexcept
		: mDevice(std::move(device))
	{}

	CommandPoolBuilder& CommandPoolBuilder::index(uint32_t index) noexcept
	{
		mIndex = index;
		return *this;
	}

	Ref<CommandPool> CommandPoolBuilder::build() const
	{
		return Ref<CommandPool>(new CommandPool(*this));
	}
}    
