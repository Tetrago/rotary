#include "rotary/core/log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace rot
{
	Logger::Logger(const std::string& name) noexcept
	{
		mLogger = spdlog::stdout_color_mt(name);

#ifndef NDEBUG
		mLogger->set_level(spdlog::level::trace);
#endif
	}

	Logger::Logger(Logger&& other) noexcept
		: mLogger(std::move(other.mLogger))
	{}

	const Logger& logger() noexcept
	{
		static Logger logger("rotary");
		return logger;
	}
}    
