#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace plat
{
	using Path = std::filesystem::path;

	std::string read_file(const Path& path);
	std::vector<uint8_t> read_binary_file(const Path& path);
}    
