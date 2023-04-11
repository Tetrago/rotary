#include "diag/logger.hpp"

#include <unordered_map>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace diag
{
	namespace
	{
		std::unordered_map<std::string, Logger> gLoggers;
	}

	Logger::Logger(const std::string& name) noexcept
	{
		mLogger = spdlog::stdout_color_mt(name);

#ifndef NDEBUG
		mLogger->set_level(spdlog::level::trace);
#endif
	}

	const Logger& logger(const std::string& name) noexcept
	{
		auto result = gLoggers.find(name);
		if(result == gLoggers.end())
		{
			gLoggers.emplace(name, Logger(name));
		}

		return gLoggers.at(name);
	}
}    
