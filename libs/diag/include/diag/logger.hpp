#pragma once

#include <string>
#include <unordered_map>
#include <string>
#include <spdlog/spdlog.h>

namespace diag
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
		~Logger() noexcept = default;

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&& other) noexcept = default;
		Logger& operator=(Logger&&) = delete;

		template<typename... Args>
		void log(LogLevel level, const fmt::format_string<Args...>& fmt, Args&&... args) const noexcept
		{
			mLogger->log(static_cast<spdlog::level::level_enum>(level), fmt, std::forward<Args>(args)...);
		}

#define FN(name) template<typename... Args> void name(const fmt::format_string<Args...>& fmt, Args&&... args) const noexcept \
		{ mLogger->name(fmt, std::forward<Args>(args)...); }
		FN(trace);
		FN(info);
		FN(warn);
		FN(error);
#undef FN

		static const Logger& logger(const std::string& name) noexcept;
	private:
		std::shared_ptr<spdlog::logger> mLogger;
		static std::unordered_map<std::string, Logger> sLogger;
	};
}    
