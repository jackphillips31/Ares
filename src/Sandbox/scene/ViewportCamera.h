#pragma once
#include <Ares.h>

class ViewportCamera : public Ares::PerspectiveCamera
{
public:
	ViewportCamera(float width, float height);

	void OnUpdate(Ares::Timestep ts) override;
	void OnEvent(Ares::Event& e) override;

	void SetTempDimensions(float width, float height);

private:
	float m_TempViewportWidth;
	float m_TempViewportHeight;
	float m_MovementSpeed = 2.0f;
	float m_RotationSpeed = 60.0f;
};