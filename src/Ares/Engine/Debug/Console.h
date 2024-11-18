#pragma once

namespace Ares {

	class Console
	{
	public:
		void AddLog(const char* fmt, spdlog::level = spdlog::level::info);
		void Draw(const char* title, bool isOpen);

	private:
		std::vector<std::pair<std::string, spdlog::level>> m_Items;
		bool m_ScrollToBottom = false;
	};

}