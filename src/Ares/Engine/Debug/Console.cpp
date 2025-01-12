#include <arespch.h>
#include "Engine/Debug/Console.h"

#include <imgui.h>

namespace Ares {

	void Console::AddLog(const char* fmt, spdlog::level severity)
	{
		m_Items.push_back({ std::string(fmt), severity });
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

			ImGui::PushTextWrapPos();

			for (const auto& item : m_Items)
			{
				switch (item.second)
				{
				case spdlog::level::trace:
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
					break;
				case spdlog::level::debug:
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.5f, 1.0f, 1.0f));
					break;
				case spdlog::level::info:
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 1.0f, 0.2f, 1.0f));
					break;
				case spdlog::level::warn:
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.2f, 1.0f));
					break;
				case spdlog::level::err:
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
					break;
				case spdlog::level::critical:
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
					break;
				default:
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				}
				ImGui::TextUnformatted(item.first.c_str());
				ImGui::PopStyleColor();
			}

			ImGui::PopTextWrapPos();

			if (m_ScrollToBottom)
				ImGui::SetScrollHereY(1.0f);

			m_ScrollToBottom = false;

			ImGui::EndChild();

			ImGui::End();
		}
	}

}