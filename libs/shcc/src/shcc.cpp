#include "shcc/shcc.hpp"

#include <stdexcept>
#include <spirv_reflect.hpp>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>

namespace shcc
{
	namespace
	{
		EShLanguage get_stage(Stage stage)
		{
			switch(stage)
			{
			default:
			case Stage::Vertex: return EShLangVertex;
			case Stage::Fragment: return EShLangFragment;
			}
		}
	}

	size_t get_size_of(DataType type) noexcept
	{
		switch(type)
		{
		default:
		case DataType::Float: return sizeof(float);
		case DataType::Int: return sizeof(int);
		}
	}

	std::string_view get_entry_point(Stage stage) noexcept
	{
		switch(stage)
		{
		default:
		case Stage::Vertex: return "vs_main";
		case Stage::Fragment: return "fs_main";
		}
	}

	Package::Package(std::shared_ptr<Runtime const> runtime, const std::string& source, const std::unordered_set<Stage>& stages)
		: mRuntime(std::move(runtime))
	{
		parse(source, stages);
		reflect();
	}

	std::vector<uint8_t> Package::compile(Stage stage, Language language) const
	{
		return mSpirv.at(stage);
	}

	void Package::parse(const std::string& source, const std::unordered_set<Stage>& stages)
	{
		// Shaders need to be destructed after program
		std::vector<glslang::TShader> shaders;
		glslang::TProgram program;

		shaders.reserve(stages.size());
		for(Stage stage : stages)
		{
			glslang::TShader& shader = shaders.emplace_back(get_stage(stage));

			const char* data = source.data();
			int size = source.size();

			shader.setStringsWithLengths(&data, &size, 1);
			shader.setEnvInput(glslang::EShSourceHlsl, get_stage(stage), glslang::EShClientVulkan, 100);
			shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
			shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
			shader.setEntryPoint(get_entry_point(stage).data());

			if(!shader.parse(GetDefaultResources(), 100, false, EShMsgDefault))
			{
				mRuntime->log(shader.getInfoLog());

				throw std::runtime_error("Failed to parse shader");
			}

			program.addShader(&shader);
		}

		if(!program.link(EShMsgDefault))
		{
			mRuntime->log(program.getInfoLog());

			throw std::runtime_error("Failed to link shader");
		}

		glslang::SpvOptions options{};
		options.optimizeSize = true;
		options.stripDebugInfo = true;
		options.disableOptimizer = false;

		for(Stage stage : stages)
		{
			std::vector<uint32_t> binary;
			glslang::GlslangToSpv(*program.getIntermediate(get_stage(stage)), binary, &options);

			const uint8_t* pBinary = reinterpret_cast<const uint8_t*>(binary.data());
			mSpirv[stage] = std::vector<uint8_t>(pBinary, pBinary + binary.size() * 4);
		}
	}

	void Package::reflect()
	{
		for(const auto& [stage, bytes] : mSpirv)
		{
			spirv_cross::CompilerReflection reflection(reinterpret_cast<const uint32_t*>(bytes.data()), bytes.size() / 4);
			spirv_cross::ShaderResources resources = reflection.get_shader_resources();

			if(stage != Stage::Vertex) continue;

			mResources.vertexInputs.resize(resources.stage_inputs.size());
			for(const spirv_cross::Resource& input : resources.stage_inputs)
			{
				spirv_cross::SPIRType type = reflection.get_type(input.base_type_id);

				VertexInput& elem = mResources.vertexInputs[reflection.get_decoration(input.id, spv::DecorationLocation)];
				elem.name = input.name;
				elem.type = [&type]()
				{
					switch(type.basetype)
					{
					case type.Float: return DataType::Float;
					case type.Int: return DataType::Int;
					default:
						throw std::runtime_error("Shader using unsupported type");
					}
				}();
				elem.count = type.vecsize;
			}
		}
	}

	Runtime::Runtime(Callback callback) noexcept
		: mCallback(std::move(callback))
	{
		glslang::InitializeProcess();
	}

	Runtime::~Runtime() noexcept
	{
		glslang::FinalizeProcess();
	}

	Package Runtime::package(const std::string& source, const std::unordered_set<Stage>& stages) const
	{
		return Package(shared_from_this(), source, stages);
	}

	void Runtime::log(const std::string& message) const noexcept
	{
		mCallback(message);
	}

	std::shared_ptr<Runtime> runtime(Callback callback) noexcept
	{
		return std::shared_ptr<Runtime>(new Runtime(std::move(callback)));
	}
}
