#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace rot
{
	std::vector<uint8_t> load_binary_file(const std::filesystem::path& path);
}    
