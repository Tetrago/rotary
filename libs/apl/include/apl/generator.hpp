#pragma once

#include <shcc/shcc.hpp>
#include <yaml-cpp/yaml.h>

#include "resource.hpp"

namespace apl
{
	class Generator
	{
	public:
		virtual ~Generator() noexcept = default;

		Generator(const Generator&) = delete;
		Generator& operator=(const Generator&) = delete;

		virtual bool resolve(const std::string& generator) const;
		virtual Resource generate(const YAML::Node& properties, const YAML::Node& node, const Resource& resource) const = 0;
	protected:
		explicit Generator(const std::string& name) noexcept;
	private:
		std::string mName;
	};

	class ShccGenerator : public Generator
	{
	public:
		ShccGenerator() noexcept;

		ShccGenerator(const ShccGenerator&) = delete;
		ShccGenerator& operator=(const ShccGenerator&) = delete;

		Resource generate(const YAML::Node& properties, const YAML::Node& node, const Resource& resource) const override;
	private:
		std::shared_ptr<shcc::Runtime> mRuntime;
	};
}    
