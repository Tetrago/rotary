#pragma once

#include <filesystem>
#include <string>
#include <memory>
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
		friend class LoggerBuilder;
	public:
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&& other) noexcept = default;
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
		explicit Logger(std::shared_ptr<spdlog::logger>&& logger) noexcept;

		std::shared_ptr<spdlog::logger> mLogger;
	};

	class LoggerBuilder
	{
		friend class Logger;
	public:
		LoggerBuilder(const std::string& name) noexcept;

		LoggerBuilder& out() noexcept;
		LoggerBuilder& file(const std::filesystem::path& path) noexcept;

		Logger build() const noexcept;
	private:
		std::string mName;
		std::vector<spdlog::sink_ptr> mSinks;
	};

	const Logger& logger() noexcept;
}    
