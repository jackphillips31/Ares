#include <arespch.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

#include "Platform/WinAPI/WinImGuiLayer.h"

#include "Engine/Core/Application.h"

namespace Ares {

	WinImGuiLayer::WinImGuiLayer()
		: ImGuiLayer("WinImGuiLayer")
	{
	}

	WinImGuiLayer::~WinImGuiLayer()
	{
	}

	void WinImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		void* window = app.GetWindow().GetNativeWindow();

		ImGui_ImplWin32_InitForOpenGL(window);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void WinImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void WinImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void WinImGuiLayer::End()
	{
		//AR_CORE_WARN("WinImGuiLayer::End");
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			HWND window = static_cast<HWND>(app.GetWindow().GetNativeWindow());
			HDC backup_hdc = GetDC(window);
			HGLRC backup_hglrc = wglGetCurrentContext();
			wglMakeCurrent(backup_hdc, backup_hglrc);

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			wglMakeCurrent(backup_hdc, backup_hglrc);
			ReleaseDC(window, backup_hdc);
		}
	}

}