#include "vtk/render_pass.hpp"

#include <stdexcept>

#include "vtk/assert.hpp"
#include "vtk/logical_device.hpp"

namespace vtk
{
	RenderPass::RenderPass(const RenderPassBuilder& builder)
		: mDevice(builder.mDevice)
	{
		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = builder.mColorAttachments.size();
		createInfo.pAttachments = builder.mColorAttachments.data();
		createInfo.subpassCount = builder.mSubpasses.size();
		createInfo.pSubpasses = builder.mSubpasses.data();

		if(vkCreateRenderPass(*mDevice, &createInfo, nullptr, &mHandle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create render pass");
		}
	}

	RenderPass::~RenderPass() noexcept
	{
		if(mHandle == VK_NULL_HANDLE) return;
		vkDestroyRenderPass(*mDevice, mHandle, nullptr);
	}

	RenderPass::RenderPass(RenderPass&& other) noexcept
		: mDevice(std::move(other.mDevice))
		, mHandle(other.mHandle)
	{
		other.mHandle = VK_NULL_HANDLE;
	}

	RenderPassBuilder::RenderPassBuilder(Ref<LogicalDevice> device) noexcept
		: mDevice(std::move(device))
	{}

	RenderPassBuilder& RenderPassBuilder::beginSubpass() noexcept
	{
		VkSubpassDescription desc{};
		desc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		mSubpasses.push_back(desc);
		return *this;
	}

	RenderPassBuilder& RenderPassBuilder::addColorAttachment(VkFormat format) noexcept
	{
		VTK_ASSERT(mSubpasses.size() != 0, "Attempting to add attachment without beginning a subpass");

		VkAttachmentDescription desc{};
		desc.format = format;
		desc.samples = VK_SAMPLE_COUNT_1_BIT;
		desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		desc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference ref{};
		ref.attachment = mColorAttachments.size();
		ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		mColorAttachments.push_back(desc);
		mColorReferences.push_back(ref);

		return *this;
	}

	RenderPassBuilder& RenderPassBuilder::endSubpass() noexcept
	{
		VTK_ASSERT(mSubpasses.size() != 0, "Attempting to finalize subpass with no beginning");

		VkSubpassDescription& desc = mSubpasses.back();
		desc.colorAttachmentCount = mColorAttachments.size() - mColorReferenceIndex;
		desc.pColorAttachments = mColorReferences.data() + mColorReferenceIndex;

		mColorReferenceIndex = mColorAttachments.size();
		return *this;
	}

	Ref<RenderPass> RenderPassBuilder::build()
	{
		VTK_ASSERT(mSubpasses.size() != 0, "Attempting to build RenderPass without any subpasses");

		return Ref<RenderPass>(new RenderPass(*this));
	}
}    
