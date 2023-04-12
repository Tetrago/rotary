#include "vtk/physical_device.hpp"

#include <cstdint>
#include <diag/diag.hpp>

#include "vtk/instance.hpp"
#include "vtk/logical_device.hpp"

namespace vtk
{
	PhysicalDevice read_physical_device(VkPhysicalDevice handle) noexcept
	{
		PhysicalDevice device{ handle };
		vkGetPhysicalDeviceProperties(handle, &device.properties);
		vkGetPhysicalDeviceFeatures(handle, &device.features);

		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(handle, &count, nullptr);
		device.queueFamilies.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(handle, &count, device.queueFamilies.data());

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
		std::erase_if(mDevices, [](const PhysicalDevice& device)
		{
			return device.properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		});

		return *this;
	}

	PhysicalDeviceSelector& PhysicalDeviceSelector::requireGraphicsSupport() noexcept
	{
		return filter([](std::span<VkQueueFamilyProperties const> queueFamilies)
		{
			return find_queue_family(queueFamilies, QueueType::Graphics).has_value();
		});
	}

	PhysicalDeviceSelector& PhysicalDeviceSelector::filter(const std::function<bool(const PhysicalDevice&)>& predicate) noexcept
	{
		std::erase_if(mDevices, std::not_fn(predicate));

		return *this;
	}

	PhysicalDeviceSelector& PhysicalDeviceSelector::filter(const std::function<bool(std::span<VkQueueFamilyProperties const>)>& predicate) noexcept
	{
		std::erase_if(mDevices, [&predicate](const PhysicalDevice& device)
		{
			return !predicate(device.queueFamilies);
		});

		return *this;
	}

	std::optional<PhysicalDevice> PhysicalDeviceSelector::select() const noexcept
	{
		return mDevices.empty() ? std::nullopt : std::optional(mDevices.front());
	}
}    
