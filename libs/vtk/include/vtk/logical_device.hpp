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
		LogicalDevice(LogicalDevice&&) = delete;
		LogicalDevice& operator=(LogicalDevice&&) = delete;

		operator VkDevice() const noexcept { return mHandle; }

		Holder<VkFence> createFence(bool signaled = false) const;
		Holder<VkSemaphore> createSemaphore() const;

		const PhysicalDevice& physicalDevice() const noexcept { return mPhysicalDevice; }
		VkQueue queue(QueueType type) const noexcept { return mQueues.at(type).second; }
		uint32_t queueIndex(QueueType type) const noexcept { return mQueues.at(type).first; }
	private:
		explicit LogicalDevice(const LogicalDeviceBuilder& builder);
		
		Ref<Instance> mInstance;
		PhysicalDevice mPhysicalDevice;
		VkDevice mHandle;
		std::unordered_map<QueueType, std::pair<uint32_t, VkQueue>> mQueues;
	};

	class LogicalDeviceBuilder
	{
		friend class LogicalDevice;
	public:
		LogicalDeviceBuilder(Ref<Instance> instance, const PhysicalDevice& device) noexcept;

		LogicalDeviceBuilder& addGraphicsQueue();
		LogicalDeviceBuilder& addPresentQueue(VkSurfaceKHR surface);
		LogicalDeviceBuilder& addSwapchainSupport() noexcept;

		Ref<LogicalDevice> build() const;
	private:
		Ref<Instance> mInstance;
		PhysicalDevice mDevice;
		std::unordered_map<QueueType, uint32_t> mQueues{};
		std::vector<std::string_view> mExtensionNames{};
	};
}    
