#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Engine/Data/Asset.h"
#include "Engine/ECS/Components/Material.h"
#include "Engine/ECS/Components/Mesh.h"

namespace Ares {

	template<typename T>
	void CombineHash(size_t& seed, const T& value)
	{
		seed ^= std::hash<T>{}(value)+0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

}

// Ares related hashes
template<>
struct std::hash<Ares::ECS::Components::Material>
{
	std::size_t operator()(const Ares::ECS::Components::Material& obj) const
	{
		size_t hash = 999666999;
		Ares::CombineHash<uint32_t>(hash, obj.m_ShaderAsset->GetAssetId());
		for (const auto& [name, asset] : obj.m_TextureAssets)
		{
			Ares::CombineHash<std::string>(hash, name);
			Ares::CombineHash<uint32_t>(hash, asset->GetAssetId());
		}
		return hash;
	}
};
template<>
struct std::hash<Ares::ECS::Components::Mesh>
{
	std::size_t operator()(const Ares::ECS::Components::Mesh& obj) const
	{
		size_t hash = 88008877;
		Ares::CombineHash<uint32_t>(hash, obj.m_MeshAsset->GetAssetId());
		return hash;
	}
};
template<>
struct std::hash<Ares::Asset>
{
	std::size_t operator()(const Ares::Asset& asset) const
	{
		size_t hash = 0;

		// Hash type
		Ares::CombineHash<std::type_index>(hash, asset.m_Type);

		// Hash filepath (if not empty)
		if (!asset.m_Filepath.empty())
		{
			Ares::CombineHash<std::string>(hash, asset.m_Filepath);
		}
		// Hash data key if filepath is empty
		else
		{
			if (asset.m_DataKey)
			{
				Ares::CombineHash<uint32_t>(hash, asset.m_DataKey);
			}
		}

		// Hash dependencies (if not empty)
		for (uint32_t id : asset.m_Dependencies)
		{
			Ares::CombineHash<uint32_t>(hash, id);
		}

		return hash;
	}

	std::size_t operator()(const Ares::Ref<Ares::Asset>& asset) const
	{
		size_t hash = 0;

		// Hash type
		Ares::CombineHash<std::type_index>(hash, asset->m_Type);

		// Hash filepath (if not empty)
		if (!asset->m_Filepath.empty())
		{
			Ares::CombineHash<std::string>(hash, asset->m_Filepath);
		}
		// Hash data key if filepath is empty
		else
		{
			if (asset->m_DataKey)
			{
				Ares::CombineHash<uint32_t>(hash, asset->m_DataKey);
			}
		}

		// Hash dependencies (if not empty)
		for (uint32_t id : asset->m_Dependencies)
		{
			Ares::CombineHash<uint32_t>(hash, id);
		}

		return hash;
	}

};

// GLM related hashes
template<>
struct std::hash<glm::vec2>
{
	std::size_t operator()(const glm::vec2& vec) const
	{
		size_t hash = 992211;
		Ares::CombineHash<float>(hash, vec.x);
		Ares::CombineHash<float>(hash, vec.y);
		return hash;
	}
};
template<>
struct std::hash<glm::vec3>
{
	std::size_t operator()(const glm::vec3& vec) const
	{
		size_t hash = 335566;
		Ares::CombineHash<float>(hash, vec.x);
		Ares::CombineHash<float>(hash, vec.y);
		Ares::CombineHash<float>(hash, vec.z);
		return hash;
	}
};
template<>
struct std::hash<glm::vec4>
{
	std::size_t operator()(const glm::vec4& vec) const
	{
		size_t hash = 339988;
		Ares::CombineHash<float>(hash, vec.x);
		Ares::CombineHash<float>(hash, vec.y);
		Ares::CombineHash<float>(hash, vec.z);
		Ares::CombineHash<float>(hash, vec.w);
		return hash;
	}
};