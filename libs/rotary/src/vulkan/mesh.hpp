#pragma once

#include <cstdint>
#include <vtk/vtk.hpp>

#include "rotary/graphics/mesh.hpp"

namespace rot
{
	class VulkanMesh : public Mesh
	{
	public:
		VulkanMesh(vtk::Ref<vtk::LogicalDevice> device, const void* pData, size_t size);

		VulkanMesh(const VulkanMesh&) = delete;
		VulkanMesh& operator=(const VulkanMesh&) = delete;

		const vtk::Buffer& buffer() const noexcept { return *mBuffer; }
	private:
		vtk::Ref<vtk::Buffer> mBuffer;
	};
}    
