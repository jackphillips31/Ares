#pragma once
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>

#include "Engine/Debug/Console.h"

namespace Ares {

	template<typename Mutex>
	class ImGuiConsoleSink : public spdlog::sinks::base_sink<Mutex>
	{
	public:
		explicit ImGuiConsoleSink(Ref<Console> console)
			: console(console)
		{
		}

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			this->formatter_->format(msg, formatted);
			console->AddLog(fmt::to_string(formatted).c_str(), msg.log_level);
		}

		void flush_() override
		{
		}

	private:
		Ref<Console> console;
	};

	using ImGuiConsoleSink_mt = ImGuiConsoleSink<std::mutex>;
	using ImGuiConsoleSink_st = ImGuiConsoleSink<spdlog::details::null_mutex>;

}