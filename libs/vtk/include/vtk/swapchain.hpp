#pragma once

#include <span>
#include <vector>
#include <vulkan/vulkan.h>

#include "memory.hpp"

namespace vtk
{
	class LogicalDevice;
	struct PhysicalDevice;

	struct SwapchainCapabilities
	{
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		std::vector<VkSurfaceFormatKHR> surfaceFormats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	SwapchainCapabilities read_swapchain_capabilities(const PhysicalDevice& device, VkSurfaceKHR surface) noexcept;

	class Swapchain
	{
		friend class SwapchainBuilder;
	public:
		~Swapchain() noexcept;

		Swapchain(const Swapchain&) = delete;
		Swapchain& operator=(const Swapchain&) = delete;
		Swapchain(Swapchain&& other) noexcept;
		Swapchain& operator=(Swapchain&&) = delete;

		operator VkSwapchainKHR() const noexcept { return mHandle; }

		std::span<VkImageView const> images() const noexcept { return mImageViews; }
		VkFormat format() const noexcept { return mFormat; }
		VkExtent2D extent() const noexcept { return mExtent; }
	private:
		explicit Swapchain(const SwapchainBuilder& builder);

		void createImageViews();

		Ref<LogicalDevice> mLogicalDevice;
		VkSwapchainKHR mHandle;
		VkFormat mFormat;
		VkExtent2D mExtent;
		std::vector<VkImage> mImages;
		std::vector<VkImageView> mImageViews;
	};

	class SwapchainBuilder
	{
		friend class Swapchain;
	public:
		SwapchainBuilder(Ref<LogicalDevice> logicalDevice, VkSurfaceKHR surface) noexcept;

		SwapchainBuilder& prefer(VkPresentModeKHR mode) noexcept;
		SwapchainBuilder& prefer(const VkExtent2D& extent) noexcept;

		Ref<Swapchain> build() const;
	private:
		VkSurfaceFormatKHR chooseSurfaceFormat() const noexcept;
		VkPresentModeKHR choosePresentMode() const noexcept;
		VkExtent2D chooseExtent() const noexcept;

		Ref<LogicalDevice> mLogicalDevice;
		VkSurfaceKHR mSurface;
		SwapchainCapabilities mCapabilities;
		VkSurfaceFormatKHR mSurfaceFormat;
		VkPresentModeKHR mPresentMode;
		VkExtent2D mExtent;
	};
}    
