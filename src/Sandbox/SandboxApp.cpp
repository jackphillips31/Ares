#include <Ares.h>
#include <Engine/Core/EntryPoint.h>

#include "Sandbox2D.h"

#include "assets/resource.h"

Ares::Application* Ares::CreateApplication()
{
	int32_t appIcon = IDI_ICON1;
	
	Ares::ApplicationSettings settings;
	settings.WindowStyle = AR_WINDOW_DEFAULT_WINDOW;
	settings.Icon = &appIcon;
	settings.ThreadCount = 4;

	Ares::Application* app = new Application(settings);
	app->PushLayer(Ares::CreateRef<Sandbox2D>(*app));

	return app;
}