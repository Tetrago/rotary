#include <stack>
#include <string>
#include <ranges>
#include <algorithm>
#include <iterator>

#include <gtest/gtest.h>
#include <plat/window.hpp>
#include <plat/vulkan.hpp>
#include <vtk/vtk.hpp>

TEST(Vulkan, Lifecycle)
{
	bool fail = false;
	auto callback = [&fail](vtk::Severity severity, const std::string& message)
	{
		if(severity != vtk::Severity::Error) return;

		ADD_FAILURE() << message;
		fail = true;
	};

	plat::Window window(256, 192, "Test");
	
	vtk::Ref<vtk::Instance> instance = vtk::InstanceBuilder()
		.application("test", 0, 1, 0)
		.engine("vtk", 0, 1, 0)
		.extensions(plat::get_required_instance_extensions())
		.debug(callback)
		.build();

	VkSurfaceKHR surface = plat::create_window_surface(window, *instance);
	
	auto physicalDevice = vtk::PhysicalDeviceSelector(*instance)
		.requireGraphicsSupport()
		.requirePresentSupport(surface)
		.select();
	EXPECT_TRUE(physicalDevice.has_value());

	vtk::Ref<vtk::LogicalDevice> device = vtk::LogicalDeviceBuilder(instance, physicalDevice.value())
		.addGraphicsQueue()
		.addPresentQueue(surface)
		.addSwapchainSupport()
		.build();

	vtk::Ref<vtk::Swapchain> swapchain = vtk::SwapchainBuilder(device, surface)
		.build();

	vtk::Ref<vtk::RenderPass> renderPass = vtk::RenderPassBuilder(device)
		.beginSubpass()
		.addColorAttachment(swapchain->format())
		.endSubpass()
		.build();

	std::vector<vtk::Ref<vtk::Framebuffer>> framebuffers(swapchain->views().size());
	for(int i = 0; i < framebuffers.size(); ++i)
	{
		framebuffers[i] = vtk::FramebufferBuilder(device, renderPass)
			.size(swapchain->extent())
			.view(swapchain->views()[i])
			.build();
	};

	vtk::Ref<vtk::CommandPool> commandPool = vtk::CommandPoolBuilder(device)
		.index(device->queueIndex(vtk::QueueType::Graphics))
		.build();

	vtk::Holder<VkSemaphore> semaphore = vtk::create_semaphore(*device);
	vtk::Holder<VkFence> fence = vtk::create_fence(*device);

	fence.reset();
	semaphore.reset();

	commandPool.reset();
	framebuffers.clear();
	renderPass.reset();
	swapchain.reset();
	device.reset();

	plat::free_window_surface(*instance, surface);
	instance.reset();

	if(fail)
		FAIL();
	else
		SUCCEED();
}
