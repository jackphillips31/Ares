#include <arespch.h>

#include "Engine/Data/AssetManager.h"

#include "Engine/ECS/Systems/RenderSystem.h"

namespace Ares::ECS::Systems {

	void RenderSystem::OnInit(Scene& scene)
	{
		if (AssetManager::GetAsset("TextureShader") == nullptr)
		{
			AssetManager::Load(
				AssetManager::Stage<ShaderProgram>("TextureShader", "assets/shaders/TextureShaderSource.shader"),
				[this](Ref<Asset> asset) {
					m_ShaderProgram = asset;
					m_ShaderSelfLoaded = true;
				}
			);
		}
	}

	void RenderSystem::OnShutdown(Scene& scene)
	{
		if (m_ShaderSelfLoaded)
		{
			AssetManager::Unload(m_ShaderProgram);
		}
		m_ShaderProgram.reset();
	}

	void RenderSystem::OnUpdate(Scene& scene, Timestep timestep)
	{

	}

	void RenderSystem::OnRender(Scene& scene)
	{
		if (m_ShaderProgram != nullptr)
		{
			//AR_CORE_TRACE("RenderSystem::OnRender: {}", m_ShaderProgram->GetName());
		}
	}

	void RenderSystem::SetShaderProgram(const Ref<Asset>& shaderAsset)
	{
		if (shaderAsset->GetState() != AssetState::Loaded)
		{
			AR_CORE_WARN("Could not set Render System's shader program! Asset: {}", shaderAsset->GetStateString());
			return;
		}
		if (shaderAsset->GetType() != typeid(ShaderProgram))
		{
			AR_CORE_WARN("Asset given to Render System is not a shader program! Asset: {}", shaderAsset->GetTypeName());
			return;
		}

		m_ShaderProgram = shaderAsset;
		m_ShaderSelfLoaded = false;
	}

}