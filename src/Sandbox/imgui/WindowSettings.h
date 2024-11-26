#pragma once
#include <Ares.h>
#include <imgui/imgui.h>

static uint16_t s_WindowSettings = 0;
static int s_WindowDimensions[2] = {1920, 1080};

static void DrawWindowSettingsWindow()
{
	ImGui::Begin("Window Settings");

	std::bitset<16> binary(s_WindowSettings);
	ImGui::Text("Binary value: %s", binary.to_string().c_str());
	ImGui::Text("Window Dimensions: %u x %u", Ares::Application::Get().GetWindow().GetWidth(), Ares::Application::Get().GetWindow().GetHeight());
	ImGui::Text("Client Dimensions: %u x %u", Ares::Application::Get().GetWindow().GetClientWidth(), Ares::Application::Get().GetWindow().GetClientHeight());
	std::pair<int32_t, int32_t> windowPos = Ares::Application::Get().GetWindow().GetWindowPos();
	ImGui::Text("Window Position: %u x %u", windowPos.first, windowPos.second);

	ImGui::SeparatorText("Combined Flags");

	bool isDefaultWindow = s_WindowSettings == AR_WINDOW_DEFAULT_WINDOW;
	if (ImGui::Checkbox("Default Window", &isDefaultWindow))
	{
		s_WindowSettings = isDefaultWindow ? AR_WINDOW_DEFAULT_WINDOW : 0;
	}

	bool isFullscreenBorderless = s_WindowSettings == AR_WINDOW_FULLSCREEN_BORDERLESS;
	if (ImGui::Checkbox("Fullscreen Borderless", &isFullscreenBorderless))
	{
		s_WindowSettings = isFullscreenBorderless ? AR_WINDOW_FULLSCREEN_BORDERLESS : 0;
	}

	bool isFullscreenWindowed = s_WindowSettings == AR_WINDOW_FULLSCREEN_WINDOWED;
	if (ImGui::Checkbox("Fullscreen Windowed", &isFullscreenWindowed))
	{
		s_WindowSettings = isFullscreenWindowed ? AR_WINDOW_FULLSCREEN_WINDOWED : 0;
	}

	ImGui::SeparatorText("Individual Flags");

	bool isWindowed = s_WindowSettings & AR_WINDOW_WINDOWED;
	if (ImGui::Checkbox("Windowed", &isWindowed))
	{
		s_WindowSettings ^= AR_WINDOW_WINDOWED;
	}

	bool isBorderless = s_WindowSettings & AR_WINDOW_BORDERLESS;
	if (ImGui::Checkbox("Borderless", &isBorderless))
	{
		s_WindowSettings ^= AR_WINDOW_BORDERLESS;
	}

	bool isFullscreen = s_WindowSettings & AR_WINDOW_FULLSCREEN;
	if (ImGui::Checkbox("Fullscreen", &isFullscreen))
	{
		s_WindowSettings ^= AR_WINDOW_FULLSCREEN;
	}

	bool isFullscreenEx = s_WindowSettings & AR_WINDOW_FULLSCREEN_EX;
	if (ImGui::Checkbox("Fullscreen Exclusive", &isFullscreenEx))
	{
		s_WindowSettings ^= AR_WINDOW_FULLSCREEN_EX;
	}

	bool isResizable = s_WindowSettings & AR_WINDOW_RESIZABLE;
	if (ImGui::Checkbox("Resizable", &isResizable))
	{
		s_WindowSettings ^= AR_WINDOW_RESIZABLE;
	}

	bool IsMaximized = s_WindowSettings & AR_WINDOW_MAXIMIZED;
	if (ImGui::Checkbox("Maximized", &IsMaximized))
	{
		s_WindowSettings ^= AR_WINDOW_MAXIMIZED;
	}

	ImGui::Separator();

	if (ImGui::Button("Set Window Settings"))
	{
		Ares::Application& app = Ares::Application::Get();
		app.GetWindow().SetWindowSettings(s_WindowSettings);
	}

	ImGui::End();
}