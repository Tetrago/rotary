#include "rotary/io/file.hpp"

#include <fstream>
#include <stdexcept>

namespace rot
{
	std::vector<uint8_t> load_binary_file(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::ate | std::ios::binary);
		if(!file.is_open())
		{
			throw std::runtime_error("Could not open file");
		}

		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> data(size);
		file.read(reinterpret_cast<char*>(data.data()), size);

		return data;
	}
}
