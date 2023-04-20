#pragma once

#include <string>
#include <spdlog/spdlog.h>

namespace rot
{
	enum class LogLevel
	{
		Trace = spdlog::level::trace,
		Info = spdlog::level::info,
		Warn = spdlog::level::warn,
		Error = spdlog::level::err
	};

	class Logger
	{
	public:
		explicit Logger(const std::string& name) noexcept;

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&& other) noexcept;
		Logger& operator=(Logger&&) = delete;

		template<typename... Args>
		void log(LogLevel level, const fmt::format_string<Args...>& fmt, Args&&... args) const noexcept
		{
			mLogger->log(static_cast<spdlog::level::level_enum>(level), fmt, std::forward<Args>(args)...);
		}

#define FN(level) template<typename... Args> void level(const fmt::format_string<Args...>& fmt, Args&&... args) const noexcept \
		{ mLogger->level(fmt, std::forward<Args>(args)...); }
		FN(trace);
		FN(info);
		FN(warn);
		FN(error);
#undef FN
	private:
		std::shared_ptr<spdlog::logger> mLogger;
	};

	const Logger& logger() noexcept;
}    
