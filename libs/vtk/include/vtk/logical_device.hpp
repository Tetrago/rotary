#pragma once

#include <cstdint>
#include <unordered_map>
#include <vulkan/vulkan.h>

#include "physical_device.hpp"

namespace vtk
{
	enum class QueueType
	{
		Graphics
	};

	std::optional<uint32_t> find_queue_family(std::span<VkQueueFamilyProperties const> queueFamilies, QueueType target) noexcept;

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
		explicit LogicalDevice(const LogicalDeviceBuilder& builder);
		
		VkDevice mHandle;
		PhysicalDevice mPhysicalDevice;
		std::unordered_map<QueueType, VkQueue> mQueues;
	};

	class LogicalDeviceBuilder
	{
		friend class LogicalDevice;
	public:
		explicit LogicalDeviceBuilder(const PhysicalDevice& device) noexcept;

		LogicalDeviceBuilder& addGraphicsQueue() noexcept;

		LogicalDevice build() const;
	private:
		PhysicalDevice const mDevice;
		std::unordered_map<QueueType, uint32_t> mQueues{};
	};
}    
