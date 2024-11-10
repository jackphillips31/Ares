#pragma once
#include <arespch.h>
#include "Ares/Core/Core.h"

#ifdef AR_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	std::cout << "Hello World!" << std::endl;

	Ares::Application* app = Ares::CreateApplication();

	app->Run();

	delete app;
}

#endif