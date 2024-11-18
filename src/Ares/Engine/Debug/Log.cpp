#include <arespch.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Engine/Debug/ImGuiConsoleSink.h"

#include "Engine/Debug/Log.h"

namespace Ares {
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;
	Ref<Console> Log::s_Console;

	void Log::Init(spdlog::level setLevel)
	{
		auto pattern = "%^[%T] %7n: %v%$";

		s_Console = CreateRef<Console>();

		Ref<ImGuiConsoleSink_mt> imgui_sink = CreateRef<ImGuiConsoleSink_mt>(s_Console);
		Ref<spdlog::sinks::stdout_color_sink_mt> stdout_sink = CreateRef<spdlog::sinks::stdout_color_sink_mt>();
		Ref<spdlog::sinks::stderr_color_sink_mt> stderr_sink = CreateRef<spdlog::sinks::stderr_color_sink_mt>();

		imgui_sink->set_pattern(pattern);
		stdout_sink->set_pattern(pattern);
		stderr_sink->set_pattern(pattern);

		imgui_sink->set_level(setLevel);
		stdout_sink->set_level(setLevel);
		stderr_sink->set_level(setLevel);

		s_CoreLogger = CreateRef<spdlog::logger>("ARES", spdlog::sinks_init_list{ stdout_sink, imgui_sink });
		s_CoreLogger->set_level(setLevel);
		spdlog::register_logger(s_CoreLogger);

		s_ClientLogger = CreateRef<spdlog::logger>("APP", spdlog::sinks_init_list{ stderr_sink, imgui_sink });
		s_ClientLogger->set_level(setLevel);
		spdlog::register_logger(s_ClientLogger);

		/*
		s_CoreLogger = spdlog::stdout_color_mt("ARES");
		s_CoreLogger->set_level(setLevel);
		s_ClientLogger = spdlog::stderr_color_mt("APP");
		s_ClientLogger->set_level(setLevel);
		*/

		AR_CORE_INFO("Logging initialized!");
	}
}