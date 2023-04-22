#pragma once

#include <cstdint>
#include <span>
#include <vector>
#include <vulkan/vulkan.h>

#include "memory.hpp"

namespace vtk
{
	class LogicalDevice;
	class RenderPass;

	class Pipeline
	{
		friend class PipelineBuilder;
	public:
		~Pipeline() noexcept;

		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

		operator VkPipeline() const noexcept { return mHandle; }
	private:
		explicit Pipeline(const PipelineBuilder& builder);

		Ref<LogicalDevice> mDevice;
		Ref<RenderPass> mRenderPass;
		VkPipelineLayout mLayout;
		VkPipeline mHandle;
	};

	class PipelineBuilder
	{
		friend class Pipeline;
	public:
		PipelineBuilder(Ref<LogicalDevice> device, Ref<RenderPass> renderPass) noexcept;

		PipelineBuilder& add(VkShaderStageFlagBits stage, std::span<uint8_t const> code) noexcept;
		PipelineBuilder& begin() noexcept;
		PipelineBuilder& input(VkFormat format, uint32_t offset) noexcept;
		PipelineBuilder& end(size_t size) noexcept;
		PipelineBuilder& topology(VkPrimitiveTopology topology) noexcept;
		PipelineBuilder& polygonMode(VkPolygonMode mode) noexcept;
		PipelineBuilder& viewport(const VkViewport& viewport) noexcept;
		PipelineBuilder& viewport(const VkExtent2D& extent) noexcept;
		PipelineBuilder& scissor(const VkRect2D& scissor) noexcept;
		PipelineBuilder& scissor(const VkExtent2D& extent) noexcept;

		Ref<Pipeline> build() const;
	private:
		Ref<LogicalDevice> mDevice;
		Ref<RenderPass> mRenderPass;
		std::vector<std::span<uint8_t const>> mModules;
		std::vector<VkPipelineShaderStageCreateInfo> mStages;
		std::vector<VkVertexInputBindingDescription> mInputBindings;
		std::vector<VkVertexInputAttributeDescription> mInputAttributes;
		int mAttributeIndex = 0;
		VkPipelineInputAssemblyStateCreateInfo mAssemblyState{};
		VkPipelineRasterizationStateCreateInfo mRasterizationState{};
		VkPipelineMultisampleStateCreateInfo mMultisampleState{};
		VkPipelineColorBlendAttachmentState mAttachmentState{};
		VkViewport mViewport;
		VkRect2D mScissor;
		VkPipelineViewportStateCreateInfo mViewportState{};
		VkPipelineColorBlendStateCreateInfo mBlendState{};
	};
}    
