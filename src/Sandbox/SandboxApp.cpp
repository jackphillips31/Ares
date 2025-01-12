#include <Ares.h>
#include <Engine/Core/EntryPoint.h>

#include "Sandbox2D.h"

#include "assets/resource.h"

class SandboxApp : public Ares::Application
{
public:
	SandboxApp(const Ares::ApplicationSettings& settings)
		: Application(settings)
	{
		PushLayer(Ares::CreateRef<Sandbox2D>());
	}
	~SandboxApp()
	{

	}
};

Ares::Application* Ares::CreateApplication()
{
	int32_t appIcon = IDI_ICON1;
	
	Ares::ApplicationSettings settings;
	settings.WindowStyle = AR_WINDOW_DEFAULT_WINDOW;
	settings.Icon = &appIcon;
	settings.ThreadCount = 4;

	return new SandboxApp(settings);
}