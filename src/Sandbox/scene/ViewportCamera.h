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
	glm::vec3 GetForwardVector() const;
	glm::vec3 GetRightVector() const;
	glm::vec3 GetUpVector() const;

private:
	float m_TempViewportWidth;
	float m_TempViewportHeight;
};