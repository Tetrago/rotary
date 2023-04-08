#pragma once

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

namespace vtk
{
	class Instance;

	struct PhysicalDevice
	{
		VkPhysicalDevice handle;
		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;
	};

	PhysicalDevice read_physical_device(VkPhysicalDevice handle) noexcept;

	class PhysicalDeviceSelector
	{
	public:
		PhysicalDeviceSelector(const Instance& instance) noexcept;

		PhysicalDeviceSelector& requiredDiscrete() noexcept;

		std::optional<PhysicalDevice> select() const noexcept;
	private:
		std::vector<PhysicalDevice> mDevices;
	};
}    
