#include <arespch.h>
#include "Engine/ECS/Components/Material.h"

#include "Engine/Core/Application.h"
#include "Engine/Data/Asset.h"
#include "Engine/Renderer/Assets/Texture.h"
#include "Engine/Renderer/Assets/Shader.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommandQueue.h"
#include "Engine/Renderer/CommandQueue/Commands.h"
#include "Engine/Utility/Hash.h"

namespace Ares::ECS::Components {

	Material::Material()
		: m_ShaderAsset(nullptr)
	{
	}

	Material::Material(const Ref<Asset>& shaderAsset)
	{
		if (shaderAsset->GetType() != typeid(ShaderProgram))
		{
			AR_CORE_ASSERT(false, "Asset must be a Shader Program!");
			return;
		}

		m_ShaderAsset = shaderAsset;
	}

	Material::Material(const Material& other)
	{
		std::shared_lock lock(other.m_Mutex);
		m_ShaderAsset = other.m_ShaderAsset;
		m_Properties = other.m_Properties;
		m_TextureAssets = other.m_TextureAssets;
	}

	Material& Material::operator=(const Material& other)
	{
		std::shared_lock lock1(other.m_Mutex, std::defer_lock);
		std::unique_lock lock2(m_Mutex, std::defer_lock);
		std::lock(lock1, lock2);

		m_ShaderAsset = other.m_ShaderAsset;
		m_Properties = other.m_Properties;
		m_TextureAssets = other.m_TextureAssets;
		return *this;
	}

	std::string Material::GetShaderName() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_ShaderAsset != nullptr)
			return m_ShaderAsset->GetName().c_str();

		return "NULL";
	}

	size_t Material::GetShaderSize() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_ShaderAsset != nullptr)
			return m_ShaderAsset->GetDataSize();

		return 0;
	}

	MaterialProperties Material::GetProperties() const
	{
		std::shared_lock lock(m_Mutex);
		return m_MaterialProperties;
	}

	ShaderProgram* Material::GetShader() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_ShaderAsset != nullptr)
			return m_ShaderAsset->GetAsset<ShaderProgram>();
		else
			return nullptr;
	}

	void Material::SetShader(const Ref<Asset>& asset)
	{
		if (asset->GetType() != typeid(ShaderProgram))
		{
			AR_CORE_ASSERT(false, "Asset must be a Shader Program!");
			return;
		}

		std::unique_lock lock(m_Mutex);
		m_ShaderAsset = asset;
	}

	template <typename PropertyType>
	void Material::SetUniformProperty(const String& name, const PropertyType& value)
	{
		std::unique_lock lock(m_Mutex);
		m_Properties[name.c_str()] = value;
	}

	void Material::SetTexture(const String& name, const Ref<Asset>& texture)
	{
		if (texture->GetType() != typeid(Texture))
		{
			AR_CORE_ASSERT(false, "Asset must be a Texture!");
			return;
		}

		std::unique_lock lock(m_Mutex);
		m_TextureAssets[name.c_str()] = texture;
	}

	void Material::SetProperties(const MaterialProperties& props)
	{
		std::unique_lock lock(m_Mutex);
		m_MaterialProperties = props;
	}

	bool Material::IsLoaded() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_ShaderAsset != nullptr && m_ShaderAsset->GetState() == AssetState::Loaded)
		{
			for (auto& texture : m_TextureAssets)
			{
				if (texture.second == nullptr || texture.second->GetState() != AssetState::Loaded)
					return false;
			}
			return true;
		}
		return false;
	}

	bool Material::IsValid() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_ShaderAsset != nullptr)
			return true;

		return false;
	}

	void Material::PreCache() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_ShaderAsset != nullptr && m_ShaderAsset->GetState() == AssetState::Staged)
		{
			m_ShaderAsset->Load();
		}
		for (auto& texture : m_TextureAssets)
		{
			if (texture.second != nullptr && texture.second->GetState() == AssetState::Staged)
			{
				m_ShaderAsset->Load();
			}
		}
	}

	void Material::Bind() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_ShaderAsset->GetState() != AssetState::Loaded)
		{
			AR_CORE_WARN("Material's Shader Program is not loaded - Not Binding!");
			return;
		}

		ShaderProgram* shader = nullptr;
		shader = m_ShaderAsset->GetAsset<ShaderProgram>();

		if (shader == nullptr)
		{
			AR_CORE_WARN("Material's asset returned nullptr - Not Binding!");
			return;
		}

		Internal::RenderCommandQueue* command = Application::Get().GetSystem<Ares::Systems::Renderer>()->RenderCommandQueue();
		command->SubmitCommand<RenderCommands::BindShader>(shader);

		// Set properties based on type
		for (const auto& [name, value] : m_Properties)
		{
			eastl::visit([this, command, shader, name](auto&& actualValue)
				{
					using UniformType = std::decay_t<decltype(actualValue)>;
					command->SubmitCommand<RenderCommands::SetMaterialUniform<const UniformType&>>(shader, name, actualValue);
				}, value);
		}

		// Bind textures
		int32_t textureUnit = 0;
		for (const auto& [name, texture] : m_TextureAssets)
		{
			if (texture->GetState() == AssetState::Loaded)
			{
				command->SubmitCommand<RenderCommands::BindTexture>(texture->GetAsset<Texture>(), textureUnit);
				command->SubmitCommand<RenderCommands::SetMaterialUniform<int32_t>>(shader, name, textureUnit);
				textureUnit++;
			}
			else
			{
				AR_CORE_WARN("Attempted to bind Texture that is not loaded yet: {} - {}", texture->GetName(), texture->GetStateString());
			}
		}
	}

	template void Material::SetUniformProperty<int32_t>(const String&, const int32_t&);
	template void Material::SetUniformProperty<float>(const String&, const float&);
	template void Material::SetUniformProperty<glm::vec2>(const String&, const glm::vec2&);
	template void Material::SetUniformProperty<glm::vec3>(const String&, const glm::vec3&);
	template void Material::SetUniformProperty<glm::mat3>(const String&, const glm::mat3&);
	template void Material::SetUniformProperty<glm::mat4>(const String&, const glm::mat4&);

}

namespace eastl {

	size_t hash<Ares::ECS::Components::Material>::operator()(const Ares::ECS::Components::Material& material) const
	{
		std::shared_lock lock(material.m_Mutex);
		size_t hash = static_cast<size_t>(material.m_ShaderAsset->GetAssetId());
		for (const auto& [name, asset] : material.m_TextureAssets)
		{
			Ares::CombineHash<uint32_t>(hash, asset->GetAssetId());
		}
		return hash;
	}

}