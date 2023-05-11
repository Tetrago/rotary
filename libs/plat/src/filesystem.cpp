#include "plat/filesystem.hpp"

#include <fstream>
#include <stdexcept>

namespace plat
{
	std::string read_file(const Path& path)
	{
		std::ifstream file(path, std::ios::ate);
		if(!file.is_open())
		{
			throw std::runtime_error("Could not open file: " + path.string());
		}

		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::string content(size, ' ');
		file.read(content.data(), size);

		return content;
	}

	std::vector<uint8_t> read_binary_file(const Path& path)
	{
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		if(!file.is_open())
		{
			throw std::runtime_error("Could not open file: " + path.string());
		}

		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> content(size);
		file.read(reinterpret_cast<char*>(content.data()), size);

		return content;
	}
}    
