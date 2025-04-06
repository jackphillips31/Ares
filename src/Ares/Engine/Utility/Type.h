#pragma once

namespace Ares::Utility {

	enum class AssetType : uint8_t
	{
		None = 0,
		VertexShader,
		FragmentShader,
		ShaderProgram,
		Texture,
		MeshData
	};

	const AssetType GetAssetType(const std::type_index& type);
	const String ExtractClassName(const std::type_index& type);

}