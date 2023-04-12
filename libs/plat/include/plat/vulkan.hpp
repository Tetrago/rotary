#pragma once

#include <string_view>
#include <vector>
#include <vulkan/vulkan.h>

namespace plat
{
	class Window;

	std::vector<std::string_view> get_required_instance_extensions() noexcept;
	VkSurfaceKHR create_window_surface(const Window& window, VkInstance instance);
}    
