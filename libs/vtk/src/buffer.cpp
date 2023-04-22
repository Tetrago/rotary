#include "vtk/buffer.hpp"

#include <cstring>
#include <stdexcept>
#include <vk_mem_alloc.h>

#include "allocator.hpp"
#include "vtk/assert.hpp"
#include "vtk/logical_device.hpp"

namespace vtk
{
	struct Buffer::Impl
	{
		VmaAllocation allocation;
	};

	Buffer::Buffer(const BufferBuilder& builder)
		: mDevice(builder.mDevice)
		, mSize(builder.mSize)
		, mImpl(new Impl())
	{
		VkBufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = mSize;
		createInfo.usage = builder.mUsage;

		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

		if(vmaCreateBuffer(mDevice->allocator(), &createInfo, &allocInfo, &mHandle, &mImpl->allocation, nullptr) != VK_SUCCESS)
		{
			delete mImpl;
			throw std::runtime_error("Failed to allocate buffer");
		}
	}

	Buffer::~Buffer() noexcept
	{
		vmaDestroyBuffer(mDevice->allocator(), mHandle, mImpl->allocation);
		delete mImpl;
	}

	void Buffer::upload(const void* pData) const noexcept
	{
		void* pMap;
		vmaMapMemory(mDevice->allocator(), mImpl->allocation, &pMap);
		memcpy(pMap, pData, mSize);
		vmaUnmapMemory(mDevice->allocator(), mImpl->allocation);
	}

	void Buffer::upload(const void* pData, size_t size, uint32_t offset) const noexcept
	{
		VTK_ASSERT(offset + size <= mSize, "Attempting to overflow buffer");

		void* pMap;
		vmaMapMemory(mDevice->allocator(), mImpl->allocation, &pMap);
		memcpy(static_cast<uint8_t*>(pMap) + offset, pData, size);
		vmaUnmapMemory(mDevice->allocator(), mImpl->allocation);
	}

	BufferBuilder::BufferBuilder(Ref<LogicalDevice> device) noexcept
		: mDevice(std::move(device))
	{}

	BufferBuilder& BufferBuilder::size(size_t size) noexcept
	{
		mSize = size;

		return *this;
	}

	BufferBuilder& BufferBuilder::usage(VkBufferUsageFlagBits usage) noexcept
	{
		mUsage = usage;

		return *this;
	}

	Ref<Buffer> BufferBuilder::build() const
	{
		VTK_ASSERT(mSize != 0, "Cannot allocate buffer of size 0");

		return Ref<Buffer>(new Buffer(*this));
	}
}
