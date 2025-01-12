#include <arespch.h>
#include "Engine/Data/Parsers/OBJParser.h"

#include "Engine/Data/DataBuffer.h"

namespace Ares {

	ParsedMeshData::ParsedMeshData(ParsedMeshData&& other) noexcept
	{
		Positions = std::move(other.Positions);
		TextureCoordinates = std::move(other.TextureCoordinates);
		Normals = std::move(other.Normals);
		Indices = std::move(other.Indices);
		Error = std::move(other.Error);
		IsValid = other.IsValid;

		other.Positions.clear();
		other.TextureCoordinates.clear();
		other.Normals.clear();
		other.Indices.clear();
		other.Error = "This Mesh Data has been moved from!";
		other.IsValid = false;
	}

	ParsedMeshData& ParsedMeshData::operator=(ParsedMeshData&& other) noexcept
	{
		Positions = std::move(other.Positions);
		TextureCoordinates = std::move(other.TextureCoordinates);
		Normals = std::move(other.Normals);
		Indices = std::move(other.Indices);
		Error = std::move(other.Error);
		IsValid = other.IsValid;

		other.Positions.clear();
		other.TextureCoordinates.clear();
		other.Normals.clear();
		other.Indices.clear();
		other.Error = "This Mesh Data has been moved from!";
		other.IsValid = false;

		return *this;
	}

	ParsedMeshData OBJParser::ParseMesh(const DataBuffer& fileBuffer)
	{
		return ParseMesh(fileBuffer.GetBuffer(), fileBuffer.GetSize());
	}

	ParsedMeshData OBJParser::ParseMesh(const void* data, const size_t size)
	{
		std::vector<float> tempPositions;
		std::vector<float> tempTexCoords;
		std::vector<float> tempNormals;
		ParsedMeshData result;

		const char* current = static_cast<const char*>(data);
		const char* end = static_cast<const char*>(data) + size;

		// Parse line by line from the memory buffer
		while (current < end)
		{
			// Find the end of the current line
			const char* lineEnd = static_cast<const char*>(memchr(current, '\n', end - current));
			// Handle last line without newline
			if (!lineEnd) lineEnd = end;

			// Create a string_view for the current line
			std::string_view line(current, lineEnd - current);

			// Skip empty lines
			if (!line.empty())
			{
				// Remove carriage return if present
				if (line.back() == '\r')
				{
					line.remove_suffix(1);
				}

				// Process the line
				if (line[0] == 'v')
				{
					if (line.length() > 1)
					{
						if (line[1] == ' ')
						{
							// Vertex Position
							float x, y, z;
							if (sscanf_s(line.data(), "v %f %f %f", &x, &y, &z) == 3)
							{
								tempPositions.push_back(x);
								tempPositions.push_back(y);
								tempPositions.push_back(z);
							}
						}
						else if (line[1] == 't')
						{
							// Texture Coordinate
							float u, v;
							if (sscanf_s(line.data(), "vt %f %f", &u, &v) == 2)
							{
								tempTexCoords.push_back(u);
								tempTexCoords.push_back(v);
							}
						}
						else if (line[1] == 'n')
						{
							// Normal
							float x, y, z;
							if (sscanf_s(line.data(), "vn %f %f %f", &x, &y, &z) == 3)
							{
								tempNormals.push_back(x);
								tempNormals.push_back(y);
								tempNormals.push_back(z);
							}
						}
					}
				}
				else if (line[0] == 'f')
				{
					// Face
					ProcessFace(std::string_view(line), tempPositions, tempTexCoords, tempNormals, result);
				}
			}

			// Move to the start of the next line
			current = lineEnd + 1;
		}

		if (result.Positions.size() == 0 || result.Indices.size() == 0)
		{
			result.IsValid = false;
			result.Error = "Position vertex data or index data was empty!";
		}
		else
		{
			result.IsValid = true;
			result.Error = "No Error!";
		}

		return result;
	}

	void OBJParser::ProcessFace(
		std::string_view line,
		const std::vector<float>& tempPositions,
		const std::vector<float>& tempTexCoords,
		const std::vector<float>& tempNormals,
		ParsedMeshData& result
	)
	{
		// Skip 'f' character
		const char* current = line.data() + 1;

		// Parse three vertices for the face
		for (uint8_t i = 0; i < 3; i++)
		{
			// Skip whitespace
			while (*current == ' ') current++;

			// Parse vertex indices
			int32_t posIndex = -1, texIndex = -1, normalIndex = -1;
			int32_t matches = 0;

			// Try different face formats
			if ((matches = sscanf_s(current, "%d/%d/%d", &posIndex, &texIndex, &normalIndex)) == 3)
			{
				// v/vt/vn format
			}
			else if ((matches = sscanf_s(current, "%d//%d", &posIndex, &normalIndex)) == 2)
			{
				// v//vn format
				texIndex = -1;
			}
			else if ((matches = sscanf_s(current, "%d/%d", &posIndex, &texIndex)) == 2)
			{
				// v/vt format
				normalIndex = -1;
			}
			else if ((matches = sscanf_s(current, "%d", &posIndex)) == 1)
			{
				// v format
				texIndex = -1;
				normalIndex = -1;
			}
			else
			{
				// Invalid format
				continue;
			}

			// Skip to next vertex or end of line
			while (*current && *current != ' ') current++;

			// Convert indices to 0-based
			posIndex--;

			// Add position data (always present)
			result.Positions.push_back(tempPositions[posIndex * 3]);
			result.Positions.push_back(tempPositions[posIndex * 3 + 1]);
			result.Positions.push_back(tempPositions[posIndex * 3 + 2]);

			// Add texture coordinates if present
			if (texIndex > 0 && !tempTexCoords.empty())
			{
				// Convert to 0-based
				texIndex--;
				while (result.TextureCoordinates.size() < result.Positions.size() / 3 * 2)
				{
					result.TextureCoordinates.push_back(tempTexCoords[texIndex * 2]);
					result.TextureCoordinates.push_back(tempTexCoords[texIndex * 2 + 1]);
				}
			}

			// Add normals if present
			if (normalIndex > 0 && !tempNormals.empty())
			{
				// Convert to 0-based
				normalIndex--;
				while (result.Normals.size() < result.Positions.size())
				{
					result.Normals.push_back(tempNormals[normalIndex * 3]);
					result.Normals.push_back(tempNormals[normalIndex * 3 + 1]);
					result.Normals.push_back(tempNormals[normalIndex * 3 + 2]);
				}
			}

			// Add index
			result.Indices.push_back(static_cast<uint32_t>(result.Positions.size() / 3 - 1));
		}
	}

}