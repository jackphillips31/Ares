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
		PushLayer(new Sandbox2D());
	}
	~SandboxApp()
	{

	}
};

Ares::Application* Ares::CreateApplication()
{
	Ares::ApplicationSettings settings;
	settings.WindowStyle = AR_WINDOW_DEFAULT_WINDOW;
	
	int32_t appIcon = IDI_ICON1;
	settings.Icon = &appIcon;

	return new SandboxApp(settings);
}