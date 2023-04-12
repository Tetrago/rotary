#pragma once

#include <span>
#include <functional>
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
		std::vector<VkQueueFamilyProperties> queueFamilies;
	};

	PhysicalDevice read_physical_device(VkPhysicalDevice handle) noexcept;

	class PhysicalDeviceSelector
	{
	public:
		PhysicalDeviceSelector(const Instance& instance) noexcept;

		PhysicalDeviceSelector& requiredDiscrete() noexcept;
		PhysicalDeviceSelector& requireGraphicsSupport() noexcept;
		PhysicalDeviceSelector& filter(const std::function<bool(const PhysicalDevice&)>& predicate) noexcept;
		PhysicalDeviceSelector& filter(const std::function<bool(std::span<VkQueueFamilyProperties const>)>& predicate) noexcept;

		std::optional<PhysicalDevice> select() const noexcept;
	private:
		std::vector<PhysicalDevice> mDevices;
	};
}    
