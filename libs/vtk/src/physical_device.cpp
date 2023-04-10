#include "vtk/physical_device.hpp"

#include <cstdint>
#include <diag/diag.hpp>

#include "vtk/instance.hpp"

namespace vtk
{
	PhysicalDevice read_physical_device(VkPhysicalDevice handle) noexcept
	{
		PhysicalDevice device{ handle };
		vkGetPhysicalDeviceProperties(handle, &device.properties);
		vkGetPhysicalDeviceFeatures(handle, &device.features);

		return device;
	}

	PhysicalDeviceSelector::PhysicalDeviceSelector(const Instance& instance) noexcept
	{
		uint32_t count;
		vkEnumeratePhysicalDevices(instance, &count, nullptr);
		std::vector<VkPhysicalDevice> devices(count);
		vkEnumeratePhysicalDevices(instance, &count, devices.data());

		mDevices.resize(count);
		std::transform(devices.begin(), devices.end(), mDevices.begin(), [](const VkPhysicalDevice& device) { return read_physical_device(device); });
	}

	PhysicalDeviceSelector& PhysicalDeviceSelector::requiredDiscrete() noexcept
	{
		std::erase_if(mDevices, [](const PhysicalDevice& dev)
		{
			return dev.properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		});

		return *this;
	}

	std::optional<PhysicalDevice> PhysicalDeviceSelector::select() const noexcept
	{
		return mDevices.empty() ? std::nullopt : std::optional(mDevices.front());
	}
}    
