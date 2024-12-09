#pragma once
#include <Ares.h>

class ViewportCamera : public Ares::PerspectiveCamera
{
public:
	ViewportCamera(float width, float height);

	void OnUpdate(Ares::Timestep ts) override;
	void OnEvent(Ares::Event& e) override;
};