#pragma once

#include <string_view>
#include <cstdint>
#include <unordered_map>
#include <vulkan/vulkan.h>

#include "memory.hpp"
#include "physical_device.hpp"

namespace vtk
{
	class Instance;

	enum class QueueType
	{
		Graphics,
		Present
	};

	class LogicalDevice
	{
		friend class LogicalDeviceBuilder;
	public:
		~LogicalDevice() noexcept;

		LogicalDevice(const LogicalDevice&) = delete;
		LogicalDevice& operator=(const LogicalDevice&) = delete;
		LogicalDevice(LogicalDevice&& other) noexcept;
		LogicalDevice& operator=(LogicalDevice&&) = delete;

		VkQueue queue(QueueType type) const { return mQueues.at(type); }
	private:
		LogicalDevice(const LogicalDeviceBuilder& builder);
		
		Ref<Instance> mInstance;
		VkDevice mHandle;
		PhysicalDevice mPhysicalDevice;
		std::unordered_map<QueueType, VkQueue> mQueues;
	};

	class LogicalDeviceBuilder
	{
		friend class LogicalDevice;
	public:
		LogicalDeviceBuilder(Ref<Instance> instance, const PhysicalDevice& device) noexcept;

		LogicalDeviceBuilder& addGraphicsQueue() noexcept;
		LogicalDeviceBuilder& addPresentQueue(VkSurfaceKHR surface) noexcept;
		LogicalDeviceBuilder& addSwapchainSupport() noexcept;

		Ref<LogicalDevice> build() const;
	private:
		Ref<Instance> mInstance;
		PhysicalDevice mDevice;
		std::unordered_map<QueueType, uint32_t> mQueues{};
		std::vector<std::string_view> mExtensionNames{};
	};
}    
