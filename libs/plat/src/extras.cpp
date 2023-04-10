#include "plat/extras.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace plat
{
	std::vector<const char*> get_required_instance_extensions() noexcept
	{
		uint32_t count;
		const char** names = glfwGetRequiredInstanceExtensions(&count);

		return std::vector(names, names + count);
	}
}    
