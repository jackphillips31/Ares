#include <arespch.h>
#include "Engine/Utility/Type.h"

#include "Engine/Renderer/Assets/MeshData.h"
#include "Engine/Renderer/Assets/Shader.h"
#include "Engine/Renderer/Assets/Texture.h"

namespace Ares::Utility {

	const std::unordered_map<std::type_index, AssetType> g_AssetTypeMap{
		{typeid(::Ares::VertexShader), AssetType::VertexShader},
		{typeid(::Ares::FragmentShader), AssetType::FragmentShader},
		{typeid(::Ares::ShaderProgram), AssetType::ShaderProgram},
		{typeid(::Ares::Texture), AssetType::Texture},
		{typeid(::Ares::MeshData), AssetType::MeshData}
	};

	const AssetType GetAssetType(const std::type_index& type)
	{
		auto it = g_AssetTypeMap.find(type);
		if (it != g_AssetTypeMap.end())
			return it->second;
		else
			return AssetType::None;
	}

	const std::string ExtractClassName(const std::type_index& type)
	{
		std::string typeName = type.name();

		// Find the last occurrence of "::"
		size_t pos = typeName.rfind("::");
		if (pos != std::string::npos)
		{
			// Return everything after "::"
			return typeName.substr(pos + 2);
		}

		// Otherwise, check if the type contains a keyword like "class" or "struct"
		if (typeName.find("class ") == 0)
		{
			// Remove "class "
			return typeName.substr(6);
		}
		else if (typeName.find("struct ") == 0)
		{
			// Remove "struct "
			return typeName.substr(7);
		}

		// If no namespace or keyword is found, return the typeName as-is
		return typeName;
	}

}