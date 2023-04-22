#include "mesh.hpp"

namespace rot
{
	VulkanMesh::VulkanMesh(vtk::Ref<vtk::LogicalDevice> device, const void* pData, size_t size)
	{
		mBuffer = vtk::BufferBuilder(std::move(device))
			.size(size)
			.build();

		mBuffer->upload(pData);
	}
}    
