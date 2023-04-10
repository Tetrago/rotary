#include "diag/logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace diag
{
	std::unordered_map<std::string, Logger> Logger::sLogger;

	Logger::Logger(const std::string& name) noexcept
	{
		mLogger = spdlog::stdout_color_mt(name);

#ifndef NDEBUG
		mLogger->set_level(spdlog::level::trace);
#endif
	}

	const Logger& Logger::logger(const std::string& name) noexcept
	{
		auto result = sLogger.find(name);
		if(result == sLogger.end())
		{
			sLogger.emplace(name, Logger(name));
		}

		return sLogger.at(name);
	}
}    
