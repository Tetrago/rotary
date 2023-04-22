#include "rotary/core/log.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace rot
{
	Logger::Logger(std::shared_ptr<spdlog::logger>&& logger) noexcept
		: mLogger(std::move(logger))
	{
#ifndef NDEBUG
		mLogger->set_level(spdlog::level::trace);
#endif
	}

	const Logger& logger() noexcept
	{
		static Logger logger = LoggerBuilder("rotary").out().build();
		return logger;
	}

	LoggerBuilder::LoggerBuilder(const std::string& name) noexcept
		: mName(name)
	{}

	LoggerBuilder& LoggerBuilder::out() noexcept
	{
		mSinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		mSinks.back()->set_pattern("[%H:%M:%S.%e] [%n] [%l] %v");

		return *this;
	}

	LoggerBuilder& LoggerBuilder::file(const std::filesystem::path& path) noexcept
	{
		mSinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string(), true));
		mSinks.back()->set_pattern("[%H:%M:%S.%e] [%l] %v");

		return *this;
	}

	Logger LoggerBuilder::build() const noexcept
	{
		return Logger(std::make_shared<spdlog::logger>(mName, mSinks.begin(), mSinks.end()));
	}
}    
