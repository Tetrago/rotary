#include "allocator.hpp"

#include "vtk/instance.hpp"
#include "vtk/logical_device.hpp"
#include "vtk/physical_device.hpp"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace vtk
{
	Allocator::Allocator(const LogicalDevice* pDevice) noexcept
	{
		VmaAllocatorCreateInfo createInfo{};
		createInfo.physicalDevice = pDevice->physicalDevice().handle;
		createInfo.device = *pDevice;
		createInfo.instance = pDevice->instance();

		vmaCreateAllocator(&createInfo, &mAllocator);
	}

	Allocator::~Allocator() noexcept
	{
		vmaDestroyAllocator(mAllocator);
	}
}    
