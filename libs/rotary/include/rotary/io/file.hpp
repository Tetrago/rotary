#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

#include "rotary/core/base.hpp"

namespace rot
{
	ROTARY_API std::vector<uint8_t> load_binary_file(const std::filesystem::path& path);
}    
