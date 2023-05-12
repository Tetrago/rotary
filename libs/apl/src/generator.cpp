#include "apl/generator.hpp"

#include <stdexcept>
#include <iterator>
#include <ranges>
#include <algorithm>

namespace apl
{
	Generator::Generator(const std::string& name) noexcept
		: mName(name)
	{}

	bool Generator::resolve(const std::string& generator) const
	{
		return generator == mName;
	}

	ShccGenerator::ShccGenerator() noexcept
		: Generator("shcc")
	{
		mRuntime = shcc::runtime([&](const std::string& message)
		{
			throw std::runtime_error("shcc runtime encountered a fatal error");
		});
	}

	Resource ShccGenerator::generate(const YAML::Node& properties, const YAML::Node& node, const Resource& resource) const
	{
		if(!resource.isString())
		{
			throw std::runtime_error("Input resource required to be text");
		}

		const YAML::Node& mode = node["mode"];
		if(!mode.IsDefined())
		{
			throw std::runtime_error("Missing mode");
		}

		shcc::Language language = [](const std::string& mode)
		{
			if(mode == "spv")
			{
				return shcc::Language::Spirv;
			}
			else
			{
				throw std::runtime_error("Unrecognized mode: " + mode);
			}
		}(mode.as<std::string>());

		const YAML::Node& labels = properties["stages"];
		if(!labels.IsDefined() || !labels.IsSequence())
		{
			throw std::runtime_error("Incomplete properties: missing stages");
		}

		std::unordered_set<shcc::Stage> stages;
		std::ranges::transform(labels | std::views::transform([](const YAML::Node& label){ return label.as<std::string>(); }),
							   std::inserter(stages, stages.end()), [](const std::string& label)
		{
			if(label == "vert")
			{
				return shcc::Stage::Vertex;
			}
			if(label == "frag")
			{
				return shcc::Stage::Fragment;
			}
			else
			{
				throw std::runtime_error("Malformed shader stage label: " + label);
			}
		});

		shcc::Package package = mRuntime->package(resource.string(), stages);
	}
}
