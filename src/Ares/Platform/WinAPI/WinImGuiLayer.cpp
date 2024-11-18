#include <arespch.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#include <glad/wgl.h>

#include "Platform/WinAPI/WinImGuiLayer.h"

#include "Engine/Core/Application.h"

namespace Ares {

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

		HGLRC mainContext = wglGetCurrentContext();
		AR_CORE_ASSERT(mainContext, "Failed to get main context for viewport!");

		HGLRC sharedContext = wglCreateContextAttribsARB(hdc, mainContext, attributes);
		AR_CORE_ASSERT(sharedContext, "Failed to create shared OpenGL context for viewport!");

		if (!wglShareLists(mainContext, sharedContext))
		{
			DWORD error2 = GetLastError();
			AR_CORE_ERROR("wglShareLists failed with error: {}", error2);
			//AR_CORE_ASSERT(false, "Failed to share OpenGL resources!");
		}

		ReleaseDC((HWND)viewport->PlatformHandle, hdc);

		viewport->RendererUserData = (void*)sharedContext;
	}

	void RenderViewport(ImGuiViewport* viewport, void* render_arg)
	{
		HDC hdc = GetDC((HWND)viewport->PlatformHandle);
		HGLRC sharedContext = (HGLRC)viewport->RendererUserData;

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

		ImGuiPlatformIO& platformIO = ImGui::GetPlatformIO();
		platformIO.Renderer_CreateWindow = CreateViewportContext;
		platformIO.Renderer_DestroyWindow = DestroyViewportContext;
		platformIO.Platform_RenderWindow = RenderViewport;

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			HWND window = (HWND)app.GetWindow().GetNativeWindow();
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