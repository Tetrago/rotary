#include "vtk/physical_device.hpp"

#include <cstdint>
#include <diag/diag.hpp>

#include "vtk/instance.hpp"
#include "vtk/logical_device.hpp"
#include "vtk/swapchain.hpp"

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

		vkEnumerateDeviceExtensionProperties(handle, nullptr, &count, nullptr);
		device.extensions.resize(count);
		vkEnumerateDeviceExtensionProperties(handle, nullptr, &count, device.extensions.data());
		
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
		return filter([](const PhysicalDevice& device)
		{
			return std::ranges::count_if(device.queueFamilies,
										[](const VkQueueFamilyProperties& props)
										{ return props.queueFlags & VK_QUEUE_GRAPHICS_BIT; })
				&& std::ranges::count_if(device.extensions,
										[](const VkExtensionProperties& props)
										{ return strcmp(props.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });
		});
	}

	PhysicalDeviceSelector& PhysicalDeviceSelector::requirePresentSupport(VkSurfaceKHR surface) noexcept
	{
		return filter([&surface](const PhysicalDevice& device)
		{
			SwapchainCapabilities cap = read_swapchain_capabilities(device, surface);
			if(cap.surfaceFormats.empty() || cap.presentModes.empty()) return false;
			
			for(uint32_t i = 0; i < device.queueFamilies.size(); ++i)
			{
				VkBool32 support = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device.handle, i, surface, &support);

				if(!support) continue;
				return true;
			}

			return false;
		});
	}

	PhysicalDeviceSelector& PhysicalDeviceSelector::filter(const std::function<bool(const PhysicalDevice&)>& predicate) noexcept
	{
		std::erase_if(mDevices, std::not_fn(predicate));
		return *this;
	}

	std::optional<PhysicalDevice> PhysicalDeviceSelector::select() const noexcept
	{
		return mDevices.empty() ? std::nullopt : std::optional(mDevices.front());
	}
}    
