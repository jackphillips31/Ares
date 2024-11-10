#include <Ares.h>
#include <Ares/Core/EntryPoint.h>

class SandboxApp : public Ares::Application
{
public:
	SandboxApp()
	{

	}
	~SandboxApp()
	{

	}
};

Ares::Application* Ares::CreateApplication()
{
	return new SandboxApp();
}