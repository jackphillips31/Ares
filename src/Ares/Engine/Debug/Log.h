#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include "Engine/Core/Core.h"
#include "Engine/Debug/Console.h"

namespace Ares {

	class Log
	{
	public:
		static void Init(spdlog::level setLevel);

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		inline static Ref<Console> GetConsole() { return s_Console; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
		static Ref<Console> s_Console;
	};

}

	// Add AR_LOG_LOCATION to Preprocessor Definitions
	// to enable printing the source location information
	// from which the log is called.

namespace LogDebug {

	template<typename T>
	inline static std::string ToString(T& t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	template<typename... Args>
	inline static std::string ToString(const std::string formatStr, Args&&... args)
	{
		return fmt::format(fmt::runtime(formatStr), std::forward<Args>(args)...);
	}

	inline static std::string ExtractFileName(const char* fullFilePath)
	{
		std::string filePath(fullFilePath);
		size_t pos = filePath.find_last_of("/\\");
		if (pos != std::string::npos)
		{
			return filePath.substr(pos + 1);
		}
		return filePath;
	}

	inline static std::string FormatFileAndLine(std::string fileName, int line)
	{
		return fmt::format("({}:{})", fileName, line);
	}

}

#ifdef AR_LOG_LOCATION
	#define AR_CORE_CRITICAL(...) ::Ares::Log::GetCoreLogger()->critical("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_ERROR(...) ::Ares::Log::GetCoreLogger()->error("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_WARN(...) ::Ares::Log::GetCoreLogger()->warn("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_INFO(...) ::Ares::Log::GetCoreLogger()->info("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_BUG(...) ::Ares::Log::GetCoreLogger()->debug("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_TRACE(...) ::Ares::Log::GetCoreLogger()->trace("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))

	#define AR_CRITICAL(...) ::Ares::Log::GetClientLogger()->critical("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_ERROR(...) ::Ares::Log::GetClientLogger()->error("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_WARN(...) ::Ares::Log::GetClientLogger()->warn("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_INFO(...) ::Ares::Log::GetClientLogger()->info("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_BUG(...) ::Ares::Log::GetClientLogger()->debug("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
	#define AR_TRACE(...) ::Ares::Log::GetClientLogger()->trace("{:^40} {:>30}: {}", __FUNCTION__, LogDebug::FormatFileAndLine(LogDebug::ExtractFileName(__FILE__), __LINE__), LogDebug::ToString(__VA_ARGS__))
#else
	#define AR_CORE_CRITICAL(...) ::Ares::Log::GetCoreLogger()->critical(LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_ERROR(...) ::Ares::Log::GetCoreLogger()->error(LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_WARN(...) ::Ares::Log::GetCoreLogger()->warn(LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_INFO(...) ::Ares::Log::GetCoreLogger()->info(LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_DEBUG(...) ::Ares::Log::GetCoreLogger()->debug(LogDebug::ToString(__VA_ARGS__))
	#define AR_CORE_TRACE(...) ::Ares::Log::GetCoreLogger()->trace(LogDebug::ToString(__VA_ARGS__))

	#define AR_CRITICAL(...) ::Ares::Log::GetClientLogger()->critical(LogDebug::ToString(__VA_ARGS__))
	#define AR_ERROR(...) ::Ares::Log::GetClientLogger()->error(LogDebug::ToString(__VA_ARGS__))
	#define AR_WARN(...) ::Ares::Log::GetClientLogger()->warn(LogDebug::ToString(__VA_ARGS__))
	#define AR_INFO(...) ::Ares::Log::GetClientLogger()->info(LogDebug::ToString(__VA_ARGS__))
	#define AR_DEBUG(...) ::Ares::Log::GetClientLogger()->debug(LogDebug::ToString(__VA_ARGS__))
	#define AR_TRACE(...) ::Ares::Log::GetClientLogger()->trace(LogDebug::ToString(__VA_ARGS__))
#endif