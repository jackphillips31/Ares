#include <arespch.h>
#include "Engine/Debug/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#include "Engine/Debug/ImGuiConsoleSink.h"

namespace Ares {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	Ref<Console> Log::s_Console;

	void Log::Init(spdlog::level setLevel)
	{
		auto pattern = "%^[%T] %7n: %v%$";

		s_Console = CreateRef<Console>();

		std::shared_ptr<ImGuiConsoleSink_mt> imgui_sink = std::make_shared<ImGuiConsoleSink_mt>(s_Console);
		std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		std::shared_ptr<spdlog::sinks::stderr_color_sink_mt> stderr_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();

		imgui_sink->set_pattern(pattern);
		stdout_sink->set_pattern(pattern);
		stderr_sink->set_pattern(pattern);

		imgui_sink->set_level(setLevel);
		stdout_sink->set_level(setLevel);
		stderr_sink->set_level(setLevel);

		s_CoreLogger = std::make_shared<spdlog::logger>("ARES", spdlog::sinks_init_list{ stdout_sink, imgui_sink });
		s_CoreLogger->set_level(setLevel);
		spdlog::register_logger(s_CoreLogger);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", spdlog::sinks_init_list{ stderr_sink, imgui_sink });
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