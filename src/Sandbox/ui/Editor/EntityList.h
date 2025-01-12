#pragma once
#include <Ares.h>

class EntityListElement : public Ares::ImGuiElement
{
public:
	EntityListElement();

	void Draw() override;
	void OnUpdate(const Ares::Timestep& ts) override;

	void SetScene(Ares::ECS::Scene* scene);

private:
	uint32_t m_CurrentEntity;
	Ares::ECS::Scene* m_CurrentScene;
	std::unordered_map<uint32_t, std::vector<std::type_index>> m_ComponentMap;
	std::unordered_map<uint32_t, glm::vec3> m_EntityRotations;
};