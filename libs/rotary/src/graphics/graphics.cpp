#include "rotary/graphics/graphics.hpp"

#include "../vulkan/graphics.hpp"

namespace rot
{
	Ref<Graphics> Graphics::make(plat::Window* pWindow)
	{
		return make_ref<VulkanGraphics>(pWindow);
	}
}    
