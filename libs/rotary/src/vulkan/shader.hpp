#pragma once

#include "rotary/graphics/shader.hpp"

#include <vtk/vtk.hpp>

namespace rot
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(vtk::Ref<vtk::Pipeline>&& pipeline) noexcept;

		VulkanShader(const VulkanShader&) = delete;
		VulkanShader& operator=(const VulkanShader&) = delete;

		const vtk::Pipeline& pipeline() const noexcept { return *mPipeline; }
	private:
		vtk::Ref<vtk::Pipeline> mPipeline;
	};
}    
