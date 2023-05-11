#pragma once

#include <cstdint>
#include <vector>
#include <shcc/shcc.hpp>
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

		Ref<Mesh> createMesh(const void* pData, size_t size) override;
		Ref<Shader> createShader(const plat::Path& path) override;

		void begin() override;
		void end() override;

		void bind(const Mesh& mesh) override;
		void bind(const Shader& shader) override;

		void draw(uint32_t count, uint32_t offset) override;
	private:
		std::shared_ptr<shcc::Runtime> mShcc;
		plat::Window* mWindow;
		vtk::Ref<vtk::Instance> mInstance;
		VkSurfaceKHR mSurface;
          vtk::Ref<vtk::LogicalDevice> mDevice;
		vtk::Ref<vtk::Swapchain> mSwapchain;
		vtk::Ref<vtk::RenderPass> mRenderPass;
		std::vector<vtk::Ref<vtk::Framebuffer>> mFramebuffers;
		Ref<vtk::CommandPool> mCommandPool;
		VkCommandBuffer mCommandBuffer;
		vtk::Holder<VkSemaphore> mPresentSemaphore;
		vtk::Holder<VkSemaphore> mRenderSemaphore;
		vtk::Holder<VkFence> mRenderFence;
		uint32_t mImageIndex;
		std::vector<Ref<Mesh>> mMeshes;
		std::vector<Ref<Shader>> mShaders;
     };
}    
