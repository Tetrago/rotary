#pragma once

#include <vulkan/vulkan.h>

#include "memory.hpp"

namespace vtk
{
	class LogicalDevice;
	class RenderPass;

	class Framebuffer
	{
		friend class FramebufferBuilder;
	public:
		~Framebuffer() noexcept;

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;

		operator VkFramebuffer() const noexcept { return mHandle; }
	private:
		Framebuffer(const FramebufferBuilder& builder);
		
		Ref<LogicalDevice> mDevice;
		Ref<RenderPass> mRenderPass;
		VkFramebuffer mHandle;
	};

	class FramebufferBuilder
	{
		friend class Framebuffer;
	public:
		FramebufferBuilder(Ref<LogicalDevice> device, Ref<RenderPass> renderPass) noexcept;

		FramebufferBuilder& size(const VkExtent2D& extent) noexcept;
		FramebufferBuilder& size(int width, int height) noexcept;
		FramebufferBuilder& view(VkImageView view) noexcept;

		Ref<Framebuffer> build();
	private:
		Ref<LogicalDevice> mDevice;
		Ref<RenderPass> mRenderPass;
		int mWidth;
		int mHeight;
		VkImageView mView;
	};
}    
