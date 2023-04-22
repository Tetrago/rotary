#pragma once

#include <vk_mem_alloc.h>

#include "vtk/memory.hpp"

namespace vtk
{
	class LogicalDevice;

	class Allocator
	{
	public:
		explicit Allocator(const LogicalDevice* pDevice) noexcept;
		~Allocator() noexcept;

		Allocator(const Allocator&) = delete;
		Allocator& operator=(const Allocator&) = delete;

		operator VmaAllocator() const noexcept { return mAllocator; }
	private:
		VmaAllocator mAllocator;
	};
}    
