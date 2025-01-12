#pragma once

namespace Ares {

	class DataBuffer;
	struct RawData;

	struct ParsedShaderData
	{
		std::string VertexSource;
		std::string FragmentSource;

		bool IsValid = false;
		std::string Error = "Error not set!";

		ParsedShaderData() {}
		~ParsedShaderData() = default;
		ParsedShaderData(ParsedShaderData&& other) noexcept;
		ParsedShaderData& operator=(ParsedShaderData&& other) noexcept;
		ParsedShaderData(const ParsedShaderData&) = delete;
		ParsedShaderData& operator=(const ParsedShaderData&) = delete;
	};

	class ShaderParser
	{
	public:
		ShaderParser() = delete;

		static ParsedShaderData ParseShaders(const DataBuffer& dataBuffer);
		static ParsedShaderData ParseShaders(const void* data, const size_t size);
	};

}