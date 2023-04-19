#include "graphics.hpp"

#include <algorithm>
#include <iterator>
#include <ranges>
#include <plat/vulkan.hpp>

#include <stdexcept>

namespace rot
{
	VulkanGraphics::VulkanGraphics(plat::Window* pWindow)
		: mWindow(pWindow)
	{
		mInstance = vtk::InstanceBuilder()
			.application("rotary", 0, 1, 0)
			.engine("rotary", 0, 1, 0)
			.extensions(plat::get_required_instance_extensions())
			.debug()
			.build();

		mSurface = plat::create_window_surface(*mWindow, *mInstance);

		auto physicalDevice = vtk::PhysicalDeviceSelector(*mInstance)
			.requiredDiscrete()
			.requireGraphicsSupport()
			.requirePresentSupport(mSurface)
			.select();
		if(!physicalDevice)
		{
			throw std::runtime_error("Could not find suitable physical device");
		}

		mDevice = vtk::LogicalDeviceBuilder(mInstance, physicalDevice.value())
			.addGraphicsQueue()
			.addPresentQueue(mSurface)
			.addSwapchainSupport()
			.build();

		mSwapchain = vtk::SwapchainBuilder(mDevice, mSurface)
			.prefer(VK_PRESENT_MODE_FIFO_KHR)
			.build();

		mRenderPass = vtk::RenderPassBuilder(mDevice)
			.beginSubpass()
			.addColorAttachment(mSwapchain->format())
			.endSubpass()
			.build();

		mFramebuffers.reserve(mSwapchain->views().size());
		std::ranges::transform(mSwapchain->views(), std::back_inserter(mFramebuffers), [&](VkImageView view)
		{
			return vtk::FramebufferBuilder(mDevice, mRenderPass)
				.size(mSwapchain->extent())
				.view(view)
				.build();
		});
	}

	VulkanGraphics::~VulkanGraphics() noexcept
	{
		mFramebuffers.clear();
		mRenderPass.reset();
		mSwapchain.reset();
		mDevice.reset();
		
		plat::free_window_surface(*mInstance, mSurface);
	}
}    
