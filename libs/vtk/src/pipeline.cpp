#include "vtk/pipeline.hpp"

#include <stdexcept>
#include <vulkan/vulkan_core.h>

#include "vtk/assert.hpp"
#include "vtk/logical_device.hpp"
#include "vtk/render_pass.hpp"

namespace vtk
{
	Pipeline::Pipeline(const PipelineBuilder& builder)
		: mDevice(builder.mDevice)
		, mRenderPass(builder.mRenderPass)
	{
		std::vector<VkShaderModule> modules(builder.mModules.size());
		std::vector<VkPipelineShaderStageCreateInfo> stages = builder.mStages;

		for(int i = 0; i < builder.mModules.size(); ++i)
		{
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = builder.mModules[i].size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(builder.mModules[i].data());

			if(vkCreateShaderModule(*mDevice, &createInfo, nullptr, &modules[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create shader module");
			}

			stages[i].module = modules[i];
		}

		VkPipelineLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		if(vkCreatePipelineLayout(*mDevice, &layoutInfo, nullptr, &mLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}

		VkGraphicsPipelineCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = stages.size();
		createInfo.pStages = stages.data();
		createInfo.pVertexInputState = &builder.mInputState;
		createInfo.pInputAssemblyState = &builder.mAssemblyState;
		createInfo.pViewportState = &builder.mViewportState;
		createInfo.pRasterizationState = &builder.mRasterizationState;
		createInfo.pMultisampleState = &builder.mMultisampleState;
		createInfo.pColorBlendState = &builder.mBlendState;
		createInfo.layout = mLayout;
		createInfo.renderPass = *mRenderPass;
		createInfo.subpass = 0;
		createInfo.basePipelineHandle = VK_NULL_HANDLE;

		if(vkCreateGraphicsPipelines(*mDevice, VK_NULL_HANDLE, 1, &createInfo, nullptr, &mHandle) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline");
		}
	}

	Pipeline::~Pipeline() noexcept
	{

	}

	PipelineBuilder::PipelineBuilder(Ref<LogicalDevice> device, Ref<RenderPass> renderPass) noexcept
		: mDevice(std::move(device))
		, mRenderPass(std::move(renderPass))
	{
		mInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		mAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		mAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		mAssemblyState.primitiveRestartEnable = VK_FALSE;

		mRasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		mRasterizationState.depthClampEnable = VK_FALSE;
		mRasterizationState.rasterizerDiscardEnable = VK_FALSE;
		mRasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
		mRasterizationState.lineWidth = 1.0f;
		mRasterizationState.cullMode = VK_CULL_MODE_NONE;
		mRasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
		mRasterizationState.depthBiasEnable = VK_FALSE;
		mRasterizationState.depthBiasConstantFactor = 0;
		mRasterizationState.depthBiasClamp = 0;
		mRasterizationState.depthBiasSlopeFactor = 0;

		mMultisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		mMultisampleState.sampleShadingEnable = VK_FALSE;
		mMultisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		mMultisampleState.minSampleShading = 1;
		mMultisampleState.alphaToCoverageEnable = VK_FALSE;
		mMultisampleState.alphaToOneEnable = VK_FALSE;

		mAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
			| VK_COLOR_COMPONENT_G_BIT
			| VK_COLOR_COMPONENT_B_BIT
			| VK_COLOR_COMPONENT_A_BIT;
		mAttachmentState.blendEnable = VK_FALSE;

		mViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		mViewportState.viewportCount = 1;
		mViewportState.pViewports = &mViewport;
		mViewportState.scissorCount = 1;
		mViewportState.pScissors = &mScissor;

		mBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		mBlendState.logicOpEnable = VK_FALSE;
		mBlendState.logicOp = VK_LOGIC_OP_COPY;
		mBlendState.attachmentCount = 1;
		mBlendState.pAttachments = &mAttachmentState;
	}

	PipelineBuilder& PipelineBuilder::add(VkShaderStageFlagBits stage, std::span<uint8_t const> code) noexcept
	{
		mModules.push_back(code);

		VkPipelineShaderStageCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		createInfo.stage = stage;
		createInfo.pName = "main";

		mStages.push_back(createInfo);

		return *this;
	}

	PipelineBuilder& PipelineBuilder::topology(VkPrimitiveTopology topology) noexcept
	{
		mAssemblyState.topology = topology;

		return *this;
	}

	PipelineBuilder& PipelineBuilder::polygonMode(VkPolygonMode mode) noexcept
	{
		mRasterizationState.polygonMode = mode;

		return *this;
	}

	PipelineBuilder& PipelineBuilder::viewport(const VkViewport& viewport) noexcept
	{
		mViewport = viewport;

		return *this;
	}

	PipelineBuilder& PipelineBuilder::viewport(const VkExtent2D& extent) noexcept
	{
		mViewport.x = 0;
		mViewport.y = 0;
		mViewport.width = extent.width;
		mViewport.height = extent.height;
		mViewport.minDepth = 0;
		mViewport.maxDepth = 1;

		return *this;
	}

	PipelineBuilder& PipelineBuilder::scissor(const VkRect2D& scissor) noexcept
	{
		mScissor = scissor;

		return *this;
	}

	PipelineBuilder& PipelineBuilder::scissor(const VkExtent2D& extent) noexcept
	{
		mScissor.offset = { 0, 0 };
		mScissor.extent = extent;

		return *this;
	}

	Ref<Pipeline> PipelineBuilder::build()
	{
		VTK_ASSERT(mModules.size() != 0, "Attempting to create pipeline without any modules");

		return Ref<Pipeline>(new Pipeline(*this));
	}
}    
