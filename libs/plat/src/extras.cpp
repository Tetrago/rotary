#include "plat/extras.hpp"

#include <span>
#include <algorithm>
#include <iterator>
#include <ranges>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace plat
{
	std::vector<std::string_view> get_required_instance_extensions() noexcept
	{
		uint32_t count;
		const char** names = glfwGetRequiredInstanceExtensions(&count);

		std::vector<std::string_view> extensions;
		std::ranges::transform(std::span(names, count), std::back_inserter(extensions), [](const char* name){ return name; });

		return extensions;
	}
}    
