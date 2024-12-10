#pragma once
#include <Ares.h>

class OrthoTestCamera : public Ares::OrthographicCamera
{
public:
	OrthoTestCamera(float width, float height);

	void OnUpdate(Ares::Timestep ts) override;
	void OnEvent(Ares::Event& e) override;

	bool OnMouseScroll(Ares::MouseScrolledEvent& e);
};