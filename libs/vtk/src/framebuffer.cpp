#include "vtk/framebuffer.hpp"

#include <stdexcept>

#include "vtk/logical_device.hpp"
#include "vtk/render_pass.hpp"

namespace vtk
{
	Framebuffer::Framebuffer(const FramebufferBuilder& builder)
		: mDevice(builder.mDevice)
		, mRenderPass(builder.mRenderPass)
	{
		VkFramebufferCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = *mRenderPass;
		createInfo.width = builder.mWidth;
		createInfo.height = builder.mHeight;
		createInfo.layers = 1;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &builder.mView;

		if(vkCreateFramebuffer(*mDevice, &createInfo, nullptr, &mHandle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create framebuffer");
		}
	}

	Framebuffer::~Framebuffer() noexcept
	{
		vkDestroyFramebuffer(*mDevice, mHandle, nullptr);
	}

	FramebufferBuilder::FramebufferBuilder(Ref<LogicalDevice> device, Ref<RenderPass> renderPass) noexcept
		: mDevice(std::move(device))
		, mRenderPass(std::move(renderPass))
	{}

	FramebufferBuilder& FramebufferBuilder::size(const VkExtent2D& extent) noexcept
	{
		mWidth = extent.width;
		mHeight = extent.height;
		
		return *this;
	}

	FramebufferBuilder& FramebufferBuilder::size(int width, int height) noexcept
	{
		mWidth = width;
		mHeight = height;

		return *this;
	}

	FramebufferBuilder& FramebufferBuilder::view(VkImageView view) noexcept
	{
		mView = view;

		return *this;
	}

	Ref<Framebuffer> FramebufferBuilder::build()
	{
		return Ref<Framebuffer>(new Framebuffer(*this));
	}
}    
