#include "shader.hpp"

namespace rot
{
	VulkanShader::VulkanShader(vtk::Ref<vtk::Pipeline>&& pipeline) noexcept
		: mPipeline(std::move(pipeline))
	{}
}    
