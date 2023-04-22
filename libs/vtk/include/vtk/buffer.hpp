#pragma once

#include <vulkan/vulkan.h>

#include "memory.hpp"

namespace vtk
{
	class LogicalDevice;

	class Buffer
	{
		friend class BufferBuilder;
	public:
		~Buffer() noexcept;

		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		void upload(const void* pData) const noexcept;
		void upload(const void* pData, size_t size, uint32_t offset) const noexcept;

		operator VkBuffer() const noexcept { return mHandle; }

		const VkBuffer* handle() const noexcept { return &mHandle; }
	private:
		struct Impl;

		explicit Buffer(const BufferBuilder& builder);

		Ref<LogicalDevice> mDevice;
		size_t mSize;
		VkBuffer mHandle;
		Impl* mImpl;
	};

	class BufferBuilder
	{
		friend class Buffer;
	public:
		explicit BufferBuilder(Ref<LogicalDevice> device) noexcept;

		BufferBuilder& size(size_t size) noexcept;
		BufferBuilder& usage(VkBufferUsageFlagBits usage) noexcept;

		Ref<Buffer> build() const;
	private:
		Ref<LogicalDevice> mDevice;
		size_t mSize = 0;
		VkBufferUsageFlagBits mUsage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	};
}    
