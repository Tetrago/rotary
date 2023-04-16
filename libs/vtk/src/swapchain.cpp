#include "vtk/swapchain.hpp"

#include <algorithm>
#include <ranges>
#include <cstdint>
#include <stdexcept>
#include <limits>
#include <numeric>
#include <diag/diag.hpp>

#include "vtk/instance.hpp"
#include "vtk/logical_device.hpp"
#include "vtk/physical_device.hpp"

namespace vtk
{
	SwapchainCapabilities read_swapchain_capabilities(const PhysicalDevice& device, VkSurfaceKHR surface) noexcept
	{
		SwapchainCapabilities capabilities{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.handle, surface, &capabilities.surfaceCapabilities);

		uint32_t count;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device.handle, surface, &count, nullptr);
		capabilities.surfaceFormats.resize(count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device.handle, surface, &count, capabilities.surfaceFormats.data());

		vkGetPhysicalDeviceSurfacePresentModesKHR(device.handle, surface, &count, nullptr);
		capabilities.presentModes.resize(count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device.handle, surface, &count, capabilities.presentModes.data());

		return capabilities;
	}

	Swapchain::Swapchain(const SwapchainBuilder& builder)
		: mLogicalDevice(builder.mLogicalDevice)
	{
		mFormat = builder.mSurfaceFormat.format;
		mExtent = builder.mExtent;

		uint32_t imageCount = builder.mCapabilities.surfaceCapabilities.minImageCount + 1;
		if(builder.mCapabilities.surfaceCapabilities.maxImageCount > 0
			&& imageCount > builder.mCapabilities.surfaceCapabilities.maxImageCount)
		{
			imageCount = builder.mCapabilities.surfaceCapabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = builder.mSurface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = mFormat;
		createInfo.imageColorSpace = builder.mSurfaceFormat.colorSpace;
		createInfo.imageExtent = mExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t indices[]{ mLogicalDevice->queueIndex(QueueType::Graphics), mLogicalDevice->queueIndex(QueueType::Present) };
		if(indices[0] != indices[1])
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = indices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = builder.mCapabilities.surfaceCapabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = builder.mPresentMode;
		createInfo.clipped = VK_TRUE;

		if(vkCreateSwapchainKHR(*mLogicalDevice, &createInfo, nullptr, &mHandle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a swapchain");
		}

		vkGetSwapchainImagesKHR(*mLogicalDevice, mHandle, &imageCount, nullptr);
		mImages.resize(imageCount);
		vkGetSwapchainImagesKHR(*mLogicalDevice, mHandle, &imageCount, mImages.data());

		createImageViews();
	}

	Swapchain::~Swapchain() noexcept
	{
		if(mHandle == VK_NULL_HANDLE) return;

		for(VkImageView view : mImageViews)
		{
			vkDestroyImageView(*mLogicalDevice, view, nullptr);
		}

		vkDestroySwapchainKHR(*mLogicalDevice, mHandle, nullptr);
	}

	Swapchain::Swapchain(Swapchain&& other) noexcept
		: mLogicalDevice(std::move(other.mLogicalDevice))
		, mHandle(other.mHandle)
		, mFormat(other.mFormat)
		, mExtent(other.mExtent)
		, mImages(std::move(other.mImages))
		, mImageViews(std::move(other.mImageViews))
	{
		other.mHandle = VK_NULL_HANDLE;
	}

	void Swapchain::createImageViews()
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = mFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		mImageViews.resize(mImages.size());
		for(int i = 0; i < mImageViews.size(); ++i)
		{
			createInfo.image = mImages[i];

			if(vkCreateImageView(*mLogicalDevice, &createInfo, nullptr, &mImageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create swapchain image view");
			}
		}
	}

	SwapchainBuilder::SwapchainBuilder(Ref<LogicalDevice> logicalDevice, VkSurfaceKHR surface) noexcept
		: mLogicalDevice(logicalDevice)
		, mSurface(surface)
		, mCapabilities(read_swapchain_capabilities(logicalDevice->physicalDevice(), surface))
	{
		mSurfaceFormat = chooseSurfaceFormat();
		mPresentMode = choosePresentMode();
		mExtent = chooseExtent();
	}

	SwapchainBuilder& SwapchainBuilder::prefer(VkPresentModeKHR mode) noexcept
	{
		if(std::ranges::count(mCapabilities.presentModes, mode))
		{
			mPresentMode = mode;
		}

		diag::logger("vtk").warn("Prefered present mode could not be found for swapchain");
		return *this;
	}

	SwapchainBuilder& SwapchainBuilder::prefer(const VkExtent2D& extent) noexcept
	{
		mExtent = extent;
		return *this;
	}

	Ref<Swapchain> SwapchainBuilder::build() const
	{
		return Ref<Swapchain>(new Swapchain(*this));
	}

	VkSurfaceFormatKHR SwapchainBuilder::chooseSurfaceFormat() const noexcept
	{
		for(const VkSurfaceFormatKHR& format : mCapabilities.surfaceFormats)
		{
			if(!(format.format == VK_FORMAT_B8G8R8A8_SRGB
				 && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)) continue;

			return format;
		}

		return mCapabilities.surfaceFormats[0];
	}

	VkPresentModeKHR SwapchainBuilder::choosePresentMode() const noexcept
	{
		for(const VkPresentModeKHR& presentMode : mCapabilities.presentModes)
		{
			if(presentMode != VK_PRESENT_MODE_MAILBOX_KHR) continue;
			return presentMode;
		}

		return mCapabilities.presentModes[0];
	}

	VkExtent2D SwapchainBuilder::chooseExtent() const noexcept
	{
		return mCapabilities.surfaceCapabilities.currentExtent;
	}
}    
