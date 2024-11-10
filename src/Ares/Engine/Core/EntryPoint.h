#pragma once
#include <arespch.h>
#include "Engine/Core/Core.h"

#ifdef AR_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Ares::Log::Init(spdlog::level::trace);

	Ares::Application* app = Ares::CreateApplication();

	app->Run();

	delete app;
}

#endif