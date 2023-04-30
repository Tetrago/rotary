#include "graphics.hpp"

#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <plat/vulkan.hpp>
#include <shcc/shcc.hpp>

#include "rotary/core/log.hpp"
#include "rotary/core/memory.hpp"
#include "mesh.hpp"
#include "shader.hpp"

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

			static Logger logger = LoggerBuilder("vtk")
#ifndef NDEBUG
				.out()
#endif
				.file("vtk.log")
				.build();

			logger.log(level, "{}", message);
		}

		void shcc_callback(const std::string& message)
		{
			static Logger logger = LoggerBuilder("shcc")
				.out()
				.build();

			logger.log(LogLevel::Error, "{}", message);
		}
	}

	VulkanGraphics::VulkanGraphics(plat::Window* pWindow)
		: mShcc(shcc::runtime(shcc_callback))
		, mWindow(pWindow)
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

		mPresentSemaphore = vtk::create_semaphore(*mDevice);
		mRenderSemaphore = vtk::create_semaphore(*mDevice);
		mRenderFence = vtk::create_fence(*mDevice, true);
	}

	VulkanGraphics::~VulkanGraphics() noexcept
	{
		vtk::wait(*mDevice, mRenderFence);

		mMeshes.clear();
		mShaders.clear();

		mRenderFence.reset();
		mRenderSemaphore.reset();
		mPresentSemaphore.reset();

		mFramebuffers.clear();
		mRenderPass.reset();
		mSwapchain.reset();
		mDevice.reset();
		
		plat::free_window_surface(*mInstance, mSurface);
	}

	Ref<Mesh> VulkanGraphics::createMesh(const void* pData, size_t size)
	{
		return mMeshes.emplace_back(make_ref<VulkanMesh>(mDevice, pData, size));
	}

	Ref<Shader> VulkanGraphics::createShader(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::ate);
		if(!file.is_open())
		{
			throw std::runtime_error("Could not open shader filer");
		}

		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::string source(size, ' ');
		file.read(source.data(), size);

		shcc::Package package = mShcc->package(source, { shcc::Stage::Vertex, shcc::Stage::Fragment });
		const shcc::Resources& resources = package.resources();

		auto vert = package.compile(shcc::Stage::Vertex, shcc::Language::Spirv);
		auto frag = package.compile(shcc::Stage::Fragment, shcc::Language::Spirv);
		
		vtk::PipelineBuilder builder = vtk::PipelineBuilder(mDevice, mRenderPass)
			.add(VK_SHADER_STAGE_VERTEX_BIT, shcc::get_entry_point(shcc::Stage::Vertex), vert)
			.add(VK_SHADER_STAGE_FRAGMENT_BIT, shcc::get_entry_point(shcc::Stage::Fragment), frag)
			.viewport(mSwapchain->extent())
			.scissor(mSwapchain->extent());

		builder.begin();
		size_t offset = 0;

		for(const shcc::VertexInput& input : resources.vertexInputs)
		{
			VkFormat format = [&input]()
			{
				if(input.count < 1 || input.count > 4)
					throw std::runtime_error("Vertex input count not supported");

				switch(input.type)
				{
				default:
					throw std::runtime_error("Vertex input format not supported");
				case shcc::DataType::Float:
					switch(input.count)
					{
					case 1: return VK_FORMAT_R32_SFLOAT;
					case 2: return VK_FORMAT_R32G32_SFLOAT;
					case 3: return VK_FORMAT_R32G32B32_SFLOAT;
					case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
					};
				case shcc::DataType::Int:
					switch(input.count)
					{
					case 1: return VK_FORMAT_R32_SINT;
					case 2: return VK_FORMAT_R32G32_SINT;
					case 3: return VK_FORMAT_R32G32B32_SINT;
					case 4: return VK_FORMAT_R32G32B32A32_SINT;
					};
				}
			}();

			builder.input(format, offset);
			offset += shcc::get_size_of(input.type) * input.count;
		}

		builder.end(offset);

		return mShaders.emplace_back(make_ref<VulkanShader>(builder.build()));
	}

	void VulkanGraphics::begin()
	{
		vtk::wait(*mDevice, mRenderFence);
		vtk::reset(*mDevice, mRenderFence);

		if(vkAcquireNextImageKHR(*mDevice, *mSwapchain, std::numeric_limits<uint64_t>::max(), mPresentSemaphore, nullptr, &mImageIndex) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to acquire next swapchain image");
		}

		vtk::reset(mCommandBuffer);
		vtk::begin(mCommandBuffer, true);

		VkRenderPassBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = *mRenderPass;
		beginInfo.renderArea.extent = mSwapchain->extent();
		beginInfo.framebuffer = *mFramebuffers[mImageIndex];

		static const VkClearValue clearValue{};
		beginInfo.clearValueCount = 1;
		beginInfo.pClearValues = &clearValue;

		vkCmdBeginRenderPass(mCommandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanGraphics::end()
	{
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
		presentInfo.pImageIndices = &mImageIndex;

		if(vkQueuePresentKHR(mDevice->queue(vtk::QueueType::Graphics), &presentInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present to swapchain");
		}
	}

	void VulkanGraphics::bind(const Mesh& mesh)
	{
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(mCommandBuffer, 0, 1, static_cast<const VulkanMesh&>(mesh).buffer().handle(), &offset);
	}

	void VulkanGraphics::bind(const Shader& shader)
	{
		vkCmdBindPipeline(mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<const VulkanShader&>(shader).pipeline());
	}

	void VulkanGraphics::draw(uint32_t count, uint32_t offset)
	{
		vkCmdDraw(mCommandBuffer, count, 1, offset, 0);
	}
}    
