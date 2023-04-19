#pragma once

#include <vector>
#include <vtk/vtk.hpp>

#include "rotary/graphics/graphics.hpp"

namespace rot
{
	class VulkanGraphics : public Graphics
	{
	public:
		VulkanGraphics(plat::Window* pWindow);
		~VulkanGraphics() noexcept;

		VulkanGraphics(const VulkanGraphics&) = delete;
		VulkanGraphics& operator=(const VulkanGraphics&) = delete;
	private:
		plat::Window* mWindow;
		vtk::Ref<vtk::Instance> mInstance;
		VkSurfaceKHR mSurface;
          vtk::Ref<vtk::LogicalDevice> mDevice;
		vtk::Ref<vtk::Swapchain> mSwapchain;
		vtk::Ref<vtk::RenderPass> mRenderPass;
		std::vector<vtk::Ref<vtk::Framebuffer>> mFramebuffers;
     };
}    
