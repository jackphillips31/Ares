#include <arespch.h>
#include <imgui.h>
#include <cstdarg>

#include "Engine/Debug/Console.h"

namespace Ares {

	void Console::AddLog(const char* fmt, ...)
	{
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, sizeof(buf), fmt, args);
		va_end(args);
		m_Items.push_back(std::string(buf));
		m_ScrollToBottom = true;
	}

	void Console::Draw(const char* title, bool isOpen)
	{
		if (isOpen)
		{
			ImGui::Begin(title);
			if (ImGui::Button("Clear"))
				m_Items.clear();

			ImGui::Separator();

			ImGui::BeginChild("ScrollableRegion", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

			for (const auto& item : m_Items)
				ImGui::TextUnformatted(item.c_str());

			if (m_ScrollToBottom)
				ImGui::SetScrollHereY(1.0f);

			m_ScrollToBottom = false;

			ImGui::EndChild();

			ImGui::End();
		}
	}

}