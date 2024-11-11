#include <Ares.h>
#include <Engine/Core/EntryPoint.h>

#include "Sandbox2D.h"

class SandboxApp : public Ares::Application
{
public:
	SandboxApp()
	{
		PushLayer(new Sandbox2D());
	}
	~SandboxApp()
	{

	}
};

Ares::Application* Ares::CreateApplication()
{
	return new SandboxApp();
}