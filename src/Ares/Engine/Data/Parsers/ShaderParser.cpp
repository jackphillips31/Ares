#include <arespch.h>
#include "Engine/Data/Parsers/ShaderParser.h"

#include "Engine/Data/DataBuffer.h"

namespace Ares {

	ParsedShaderData::ParsedShaderData(ParsedShaderData&& other) noexcept
	{
		VertexSource = std::move(other.VertexSource);
		FragmentSource = std::move(other.FragmentSource);
		Error = std::move(other.Error);
		IsValid = other.IsValid;

		other.VertexSource = "";
		other.FragmentSource = "";
		other.Error = "This Shader Data has been moved from.";
		other.IsValid = false;
	}

	ParsedShaderData& ParsedShaderData::operator=(ParsedShaderData&& other) noexcept
	{
		VertexSource = std::move(other.VertexSource);
		FragmentSource = std::move(other.FragmentSource);
		Error = std::move(other.Error);
		IsValid = other.IsValid;

		other.VertexSource = "";
		other.FragmentSource = "";
		other.Error = "This Shader Data has been moved from.";
		other.IsValid = false;

		return *this;
	}

	ParsedShaderData ShaderParser::ParseShaders(const DataBuffer& dataBuffer)
	{
		return ParseShaders(dataBuffer.GetBuffer(), dataBuffer.GetSize());
	}

	ParsedShaderData ShaderParser::ParseShaders(const void* data, const size_t size)
	{
		ParsedShaderData result;

		std::string_view shaderSource(static_cast<const char*>(data), size);

		// Parse shader types from the source code
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = shaderSource.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t endOfLine = shaderSource.find_first_of("\r\n", pos);
			if (endOfLine == std::string::npos)
			{
				result.IsValid = false;
				result.Error = "Shader Syntax Error!";
				break;
			}

			size_t beginOfToken = pos + typeTokenLength + 1;
			std::string_view type = shaderSource.substr(beginOfToken, endOfLine - beginOfToken);
			if (type != "vertex" && type != "fragment" && type != "pixel")
			{
				result.IsValid = false;
				result.Error = "Shader Syntax Error: Invalid shader type specification!";
				break;
			}

			size_t nextLinePos = shaderSource.find_first_not_of("\r\n", endOfLine);
			if (nextLinePos == std::string::npos)
			{
				result.IsValid = false;
				result.Error = "Shader Syntax Error!";
				break;
			}

			pos = shaderSource.find(typeToken, nextLinePos);

			if (type == "vertex")
			{
				result.VertexSource = (pos == std::string::npos) ? shaderSource.substr(nextLinePos) : shaderSource.substr(nextLinePos, pos - nextLinePos);
			}
			else if (type == "fragment" || type == "pixel")
			{
				result.FragmentSource = (pos == std::string::npos) ? shaderSource.substr(nextLinePos) : shaderSource.substr(nextLinePos, pos - nextLinePos);
			}

			result.IsValid = true;
			result.Error = "No Error!";
		}
		return result;
	}

}