#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

#include "memory.hpp"

namespace vtk
{
	class LogicalDevice;

	class RenderPass
	{
		friend class RenderPassBuilder;
	public:
		~RenderPass() noexcept;

		RenderPass(const RenderPass&) = delete;
		RenderPass& operator=(const RenderPass&) = delete;
		RenderPass(RenderPass&&) = delete;
		RenderPass& operator=(RenderPass&&) = delete;

		operator VkRenderPass() const noexcept { return mHandle; }
	private:
		RenderPass(const RenderPassBuilder& builder);

		Ref<LogicalDevice> mDevice;
		VkRenderPass mHandle;
	};

	class RenderPassBuilder
	{
		friend class RenderPass;
	public:
		explicit RenderPassBuilder(Ref<LogicalDevice> device) noexcept;

		RenderPassBuilder& beginSubpass() noexcept;
		RenderPassBuilder& addColorAttachment(VkFormat format) noexcept;
		RenderPassBuilder& endSubpass() noexcept;

		Ref<RenderPass> build();
	private:
		Ref<LogicalDevice> mDevice;
		std::vector<VkAttachmentDescription> mColorAttachments;
		std::vector<VkAttachmentReference> mColorReferences;
		std::vector<VkSubpassDescription> mSubpasses;
		int mColorReferenceIndex = 0;
	};
}    
