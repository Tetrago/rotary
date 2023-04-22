#include "rotary/graphics/graphics.hpp"

#ifdef ROTARY_VULKAN
#include "vulkan/graphics.hpp"
#endif

namespace rot
{
	Ref<Graphics> Graphics::make(plat::Window* pWindow)
	{
#ifdef ROTARY_VULKAN
		return make_ref<VulkanGraphics>(pWindow);
#endif
	}
}    
