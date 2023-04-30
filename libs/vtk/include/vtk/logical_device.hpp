#pragma once

#include <string_view>
#include <cstdint>
#include <unordered_map>
#include <vulkan/vulkan.h>

#include "memory.hpp"
#include "physical_device.hpp"

namespace vtk
{
	class Allocator;
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

		operator VkDevice() const noexcept { return mHandle; }

		const Instance& instance() const noexcept { return *mInstance; }
		const PhysicalDevice& physicalDevice() const noexcept { return mPhysicalDevice; }
		VkQueue queue(QueueType type) const noexcept { return mQueues.at(type).second; }
		uint32_t queueIndex(QueueType type) const noexcept { return mQueues.at(type).first; }
		const Allocator& allocator() const noexcept { return *mAllocator; }
	private:
		explicit LogicalDevice(const LogicalDeviceBuilder& builder);
		
		Ref<Instance> mInstance;
		PhysicalDevice mPhysicalDevice;
		VkDevice mHandle;
		std::unordered_map<QueueType, std::pair<uint32_t, VkQueue>> mQueues;
		Ref<Allocator> mAllocator;
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
