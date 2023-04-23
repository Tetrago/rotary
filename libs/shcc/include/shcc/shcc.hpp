#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace shcc
{
	enum class DataType
	{
		Float,
		Int
	};

	size_t get_size_of(DataType type) noexcept;

	struct VertexInput
	{
		std::string name;
		DataType type;
		int count;
	};

	struct ReflectionData
	{
		std::vector<VertexInput> vertexInputs;
	};

	ReflectionData compile_reflection_data(std::span<uint8_t const> bytes);
}
