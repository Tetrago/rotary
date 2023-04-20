#include "graphics.hpp"

#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <plat/vulkan.hpp>

#include "rotary/core/log.hpp"

namespace rot
{
	namespace
	{
		void debug_callback(vtk::Severity severity, const std::string& message)
		{
			LogLevel level = [&severity]()
			{
				switch(severity)
				{
				case vtk::Severity::Trace: return LogLevel::Trace;
				case vtk::Severity::Info: return LogLevel::Info;
				case vtk::Severity::Warn: return LogLevel::Warn;
				default:
				case vtk::Severity::Error: return LogLevel::Error;
				}
			}();

			logger().log(level, "[vtk] {}", message);
		}
	}

	VulkanGraphics::VulkanGraphics(plat::Window* pWindow)
		: mWindow(pWindow)
	{
		mInstance = vtk::InstanceBuilder()
			.application("rotary", 0, 1, 0)
			.engine("rotary", 0, 1, 0)
			.extensions(plat::get_required_instance_extensions())
			.debug(debug_callback)
			.build();

		mSurface = plat::create_window_surface(*mWindow, *mInstance);

		auto physicalDevice = vtk::PhysicalDeviceSelector(*mInstance)
			.requireDiscrete()
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

		mCommandPool = vtk::CommandPoolBuilder(mDevice)
			.index(mDevice->queueIndex(vtk::QueueType::Graphics))
			.build();

		mCommandBuffer = mCommandPool->create();

		mPresentSemaphore = mDevice->createSemaphore();
		mRenderSemaphore = mDevice->createSemaphore();
		mRenderFence = mDevice->createFence(true);
	}

	VulkanGraphics::~VulkanGraphics() noexcept
	{
		vkDeviceWaitIdle(*mDevice);

		mRenderFence.reset();
		mRenderSemaphore.reset();
		mPresentSemaphore.reset();

		mFramebuffers.clear();
		mRenderPass.reset();
		mSwapchain.reset();
		mDevice.reset();
		
		plat::free_window_surface(*mInstance, mSurface);
	}

	void VulkanGraphics::begin()
	{
		
	}

	void VulkanGraphics::end()
	{
		vtk::wait(mDevice, mRenderFence);
		vtk::reset(mDevice, mRenderFence);

		uint32_t imageIndex;
		if(vkAcquireNextImageKHR(*mDevice, *mSwapchain, std::numeric_limits<uint64_t>::max(), mPresentSemaphore, nullptr, &imageIndex) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to acquire next swapchain image");
		}

		vtk::reset(mCommandBuffer);
		vtk::begin(mCommandBuffer, true);

		VkRenderPassBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = *mRenderPass;
		beginInfo.renderArea.extent = mSwapchain->extent();
		beginInfo.framebuffer = *mFramebuffers[imageIndex];

		static const VkClearValue clearValue{};
		beginInfo.clearValueCount = 1;
		beginInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(mCommandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdEndRenderPass(mCommandBuffer);

		vtk::end(mCommandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		static const VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		submitInfo.pWaitDstStageMask = &waitStage;

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = mPresentSemaphore;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = mRenderSemaphore;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &mCommandBuffer;

		if(vkQueueSubmit(mDevice->queue(vtk::QueueType::Graphics), 1, &submitInfo, mRenderFence) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit command buffer to queue");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		VkSwapchainKHR handle = *mSwapchain;
		presentInfo.pSwapchains = &handle;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = mRenderSemaphore;
		presentInfo.pImageIndices = &imageIndex;

		if(vkQueuePresentKHR(mDevice->queue(vtk::QueueType::Graphics), &presentInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present to swapchain");
		}
	}
}    
