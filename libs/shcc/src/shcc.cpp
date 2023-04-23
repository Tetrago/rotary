#include "shcc/shcc.hpp"

#include <stdexcept>
#include <spirv_reflect.hpp>

namespace shcc
{
	size_t get_size_of(DataType type) noexcept
	{
		switch(type)
		{
		default:
		case DataType::Float: return sizeof(float);
		case DataType::Int: return sizeof(int);
		}
	}

	ReflectionData compile_reflection_data(std::span<uint8_t const> bytes)
	{
		spirv_cross::CompilerReflection compiler(reinterpret_cast<const uint32_t*>(bytes.data()), bytes.size() / 4);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		ReflectionData data{};

		data.vertexInputs.resize(resources.stage_inputs.size());
		for(auto& resource : resources.stage_inputs)
		{
			auto type = compiler.get_type(resource.base_type_id);

			VertexInput input{};
			input.name = resource.name;
			input.type = [&type]()
			{
				switch(type.basetype)
				{
				default:
					throw std::runtime_error("Unsupported vertex input type");
				case type.Float: return DataType::Float;
				case type.Int: return DataType::Int;
				}
			}();
			input.count = type.vecsize;

			int location = compiler.get_decoration(resource.id, spv::DecorationLocation);
			data.vertexInputs[location] = input;
		}

		return data;
	}
}
