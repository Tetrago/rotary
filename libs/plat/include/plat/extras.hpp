#pragma once

#include <string_view>
#include <vector>

namespace plat
{
	std::vector<std::string_view> get_required_instance_extensions() noexcept;
}    
