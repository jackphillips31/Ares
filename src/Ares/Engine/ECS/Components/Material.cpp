#include <arespch.h>
#include "Engine/ECS/Components/Material.h"

#include "Engine/Data/AssetManager.h"
#include "Engine/Renderer/Assets/Texture.h"
#include "Engine/Renderer/Assets/Shader.h"

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
		std::shared_lock lock(other.m_Mutex);
		std::unique_lock lock1(m_Mutex);
		m_ShaderAsset = other.m_ShaderAsset;
		m_Properties = other.m_Properties;
		m_TextureAssets = other.m_TextureAssets;
		return *this;
	}

	std::string Material::GetShaderName() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_ShaderAsset != nullptr)
			return m_ShaderAsset->GetName();

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
	void Material::SetUniformProperty(const std::string& name, const PropertyType& value)
	{
		std::unique_lock lock(m_Mutex);
		m_Properties[name] = value;
	}

	void Material::SetTexture(const std::string& name, const Ref<Asset>& texture)
	{
		if (texture->GetType() != typeid(Texture))
		{
			AR_CORE_ASSERT(false, "Asset must be a Texture!");
			return;
		}

		std::unique_lock lock(m_Mutex);
		m_TextureAssets[name] = texture;
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
			AssetManager::Load(m_ShaderAsset);
		}
		for (auto& texture : m_TextureAssets)
		{
			if (texture.second != nullptr && texture.second->GetState() == AssetState::Staged)
			{
				AssetManager::Load(texture.second);
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

		shader->Bind();

		// Set properties based on type
		for (const auto& [name, value] : m_Properties)
		{
			std::visit([this, shader, &name](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, int32_t>) shader->SetInt(name, arg);
				else if constexpr (std::is_same_v<T, float>) shader->SetFloat(name, arg);
				else if constexpr (std::is_same_v<T, glm::vec2>) shader->SetFloat2(name, arg);
				else if constexpr (std::is_same_v<T, glm::vec3>) shader->SetFloat3(name, arg);
				else if constexpr (std::is_same_v<T, glm::vec4>) shader->SetFloat4(name, arg);
				else if constexpr (std::is_same_v<T, glm::mat3>) shader->SetMat3(name, arg);
				else if constexpr (std::is_same_v<T, glm::mat4>) shader->SetMat4(name, arg);
			}, value);
		}

		// Bind textures
		int32_t textureUnit = 0;
		for (const auto& [name, texture] : m_TextureAssets)
		{
			if (texture->GetState() == AssetState::Loaded)
			{
				texture->GetAsset<Texture>()->Bind(textureUnit);
				shader->SetInt(name, textureUnit);
				textureUnit++;
			}
			else
			{
				AR_CORE_WARN("Attempted to bind Texture that is not loaded yet: {} - {}", texture->GetName(), texture->GetStateString());
			}
		}
	}

	template void Material::SetUniformProperty<int32_t>(const std::string&, const int32_t&);
	template void Material::SetUniformProperty<float>(const std::string&, const float&);
	template void Material::SetUniformProperty<glm::vec2>(const std::string&, const glm::vec2&);
	template void Material::SetUniformProperty<glm::vec3>(const std::string&, const glm::vec3&);
	template void Material::SetUniformProperty<glm::mat3>(const std::string&, const glm::mat3&);
	template void Material::SetUniformProperty<glm::mat4>(const std::string&, const glm::mat4&);

}