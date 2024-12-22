#pragma once

#include "Engine/ECS/Core/System.h"

namespace Ares::ECS::Systems {

	class RenderSystem : public System
	{
	public:
		void OnInit(Scene& scene);
		void OnShutdown(Scene& scene);

		void OnUpdate(Scene& scene, Timestep timestep) override;
		void OnRender(Scene& scene);

		void SetShaderProgram(const Ref<Asset>& shaderAsset);
		inline ShaderProgram* GetShaderProgram() { return m_ShaderProgram->GetAsset<ShaderProgram>().get(); }

	private:
		Ref<Asset> m_ShaderProgram = nullptr;
		bool m_ShaderSelfLoaded = false;
	};

}