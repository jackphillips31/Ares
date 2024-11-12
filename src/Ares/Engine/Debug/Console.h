#pragma once

namespace Ares {

	class Console
	{
	public:
		void AddLog(const char* fmt, ...);
		void Draw(const char* title, bool isOpen);

	private:
		std::vector<std::string> m_Items;
		bool m_ScrollToBottom = false;
	};

}