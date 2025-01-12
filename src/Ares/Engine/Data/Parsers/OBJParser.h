#pragma once

namespace Ares {

	class DataBuffer;
	struct RawData;

	struct ParsedMeshData
	{
		std::vector<float> Positions;
		std::vector<float> TextureCoordinates;
		std::vector<float> Normals;
		std::vector<uint32_t> Indices;

		bool IsValid = false;
		std::string Error = "Error not set!";

		ParsedMeshData() {}
		~ParsedMeshData() = default;
		ParsedMeshData(ParsedMeshData&& other) noexcept;
		ParsedMeshData& operator=(ParsedMeshData&& other) noexcept;
		ParsedMeshData(const ParsedMeshData&) = delete;
		ParsedMeshData& operator=(const ParsedMeshData&) = delete;
	};

	class OBJParser
	{
	public:
		OBJParser() = delete;

		static ParsedMeshData ParseMesh(const DataBuffer& fileBuffer);
		static ParsedMeshData ParseMesh(const void* data, const size_t size);

	private:
		static void ProcessFace(
			std::string_view line,
			const std::vector<float>& tempPositions,
			const std::vector<float>& tempTexCoords,
			const std::vector<float>& tempNormals,
			ParsedMeshData& result
		);
	};

}