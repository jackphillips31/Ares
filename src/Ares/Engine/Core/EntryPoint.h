#pragma once

#include "Engine/Core/Core.h"

#ifdef AR_PLATFORM_WINDOWS

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return EntryPoint(__argc, __argv);
}

int main(int argc, char** argv)
{
	return EntryPoint(argc, argv);
}

#endif

int EntryPoint(int argc, char** argv)
{
	Ares::Log::Init(spdlog::level::trace);

	Ares::Application* app = Ares::CreateApplication();

	app->Run();

	delete app;

	return 0;
}