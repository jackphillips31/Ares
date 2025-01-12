#include <arespch.h>
#include "Platform/WinAPI/WinImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_win32.h>
#include <glad/wgl.h>

#include "Engine/Core/Application.h"
#include "Engine/Core/Window.h"
#include "Engine/Renderer/GraphicsContext.h"

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

	#ifdef AR_CORE_DEBUG
		io.ConfigDebugIsDebuggerPresent = true;
	#endif

		ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		platformIO.Renderer_CreateWindow = CreateViewportContext;
		platformIO.Renderer_DestroyWindow = DestroyViewportContext;
		platformIO.Platform_RenderWindow = RenderViewport;

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

		style.FrameRounding = 2.0f;
		style.WindowBorderSize = 0.0f;
		style.FrameBorderSize = 0.0f;
		style.TabBorderSize = 0.0f;
		style.TabBarBorderSize = 0.0f;
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.43f, 0.43f, 0.43f, 0.57f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.51f, 0.50f, 0.80f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.62f, 0.60f, 0.89f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.38f, 0.69f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.51f, 0.50f, 0.80f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.50f, 0.80f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.38f, 0.69f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.39f, 0.39f, 0.54f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.54f);
		colors[ImGuiCol_Header] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.39f, 0.39f, 0.39f, 0.54f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.54f);
		colors[ImGuiCol_Separator] = ImVec4(0.51f, 0.50f, 0.80f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.51f, 0.50f, 0.80f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.50f, 0.80f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.51f, 0.50f, 0.80f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.62f, 0.61f, 0.89f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.38f, 0.69f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_TabSelected] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.51f, 0.50f, 0.80f, 0.00f);
		colors[ImGuiCol_TabDimmed] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
		colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.51f, 0.50f, 0.80f, 0.43f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextLink] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavCursor] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
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
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(
			static_cast<float>(app.GetWindow().GetClientWidth()),
			static_cast<float>(app.GetWindow().GetClientHeight())
		);

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			HWND window = (HWND)app.GetWindow().GetNativeWindow();
			HDC backup_hdc = GetDC(window);
			HGLRC backup_hglrc = wglGetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			wglMakeCurrent(backup_hdc, backup_hglrc);
			ReleaseDC(window, backup_hdc);
		}
	}

	void CreateViewportContext(ImGuiViewport* viewport)
	{
		HDC hdc = GetDC((HWND)viewport->PlatformHandle);
		AR_CORE_ASSERT(hdc, "Failed to get device context for viewport pixel format!");

		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		if (pixelFormat == 0 || SetPixelFormat(hdc, pixelFormat, &pfd) == 0)
		{
			AR_CORE_ASSERT(false, "Failed to set a compatible pixel format for viewport!");
		}

		int attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_FLAGS_ARB,
			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		HGLRC mainContext = static_cast<HGLRC>(Application::Get().GetWindow().GetGraphicsContext()->GetContextHandle());
		AR_CORE_ASSERT(mainContext, "Failed to get main context for viewport!");

		HGLRC sharedContext = wglCreateContextAttribsARB(hdc, mainContext, attributes);
		AR_CORE_ASSERT(sharedContext, "Failed to create shared OpenGL context for viewport!");

		if (!wglShareLists(mainContext, sharedContext))
		{
			DWORD error = GetLastError();
			AR_CORE_ERROR("wglShareLists failed with error: {}", error);
		}

		ReleaseDC(static_cast<HWND>(viewport->PlatformHandle), hdc);

		viewport->RendererUserData = static_cast<void*>(sharedContext);
	}

	void RenderViewport(ImGuiViewport* viewport, void* render_arg)
	{
		HDC hdc = GetDC(static_cast<HWND>(viewport->PlatformHandle));
		HGLRC sharedContext = static_cast<HGLRC>(viewport->RendererUserData);

		wglMakeCurrent(hdc, sharedContext);

		ImGui_ImplOpenGL3_RenderDrawData(viewport->DrawData);

		SwapBuffers(hdc);
		
		wglMakeCurrent(nullptr, nullptr);
	}

	void DestroyViewportContext(ImGuiViewport* viewport)
	{
		HGLRC sharedContext = (HGLRC)viewport->RendererUserData;
		if (sharedContext)
		{
			wglDeleteContext(sharedContext);
			viewport->RendererUserData = nullptr;
		}
	}

}