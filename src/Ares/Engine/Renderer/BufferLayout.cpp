#include <arespch.h>
#include "Engine/Renderer/BufferLayout.h"

namespace Ares {

	ShaderDataType VertexDataToShaderData(VertexDataType type)
	{
		switch (type)
		{
		case VertexDataType::Position:		return ShaderDataType::Float3;
		case VertexDataType::PositionVec2:	return ShaderDataType::Float2;
		case VertexDataType::TextureCoords:	return ShaderDataType::Float2;
		case VertexDataType::Normal:		return ShaderDataType::Float3;
		case VertexDataType::ColorRGB:		return ShaderDataType::Float3;
		case VertexDataType::ColorRGBA:		return ShaderDataType::Float4;
		case VertexDataType::Transform:		return ShaderDataType::Mat4;
		case VertexDataType::Alpha:			return ShaderDataType::Float;
		case VertexDataType::Roughness:		return ShaderDataType::Float;
		case VertexDataType::Metallic:		return ShaderDataType::Float;
		case VertexDataType::Reflectivity:	return ShaderDataType::Float;
		case VertexDataType::EmissiveColor:	return ShaderDataType::Float3;
		case VertexDataType::EmissiveIntensity: return ShaderDataType::Float;
		case VertexDataType::ReceiveShadows:	return ShaderDataType::Int;
		case VertexDataType::CastShadows:		return ShaderDataType::Int;
		case VertexDataType::Wireframe:			return ShaderDataType::Int;
		}

		AR_CORE_ASSERT(false, "Unknown VertexDataType!");
		return ShaderDataType::None;
	}

	size_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}

		AR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	size_t UnitDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4;
		case ShaderDataType::Float3:	return 4;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 4;
		case ShaderDataType::Mat4:		return 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4;
		case ShaderDataType::Int3:		return 4;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
		}

		AR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	BufferElement::BufferElement(VertexDataType type, bool instanced, bool normalized)
		: VertexType(type), DataType(VertexDataToShaderData(type)), Size(ShaderDataTypeSize(DataType)), Offset(0), UnitSize(UnitDataTypeSize(DataType)), Instanced(instanced), Normalized(normalized)
	{
	}

	uint32_t BufferElement::GetComponentCount() const
	{
		switch (DataType)
		{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
		}

		AR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	BufferLayout::BufferLayout()
	{
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elements(elements)
	{
		CalculateOffsetAndStride();
	}

	void BufferLayout::CalculateOffsetAndStride()
	{
		size_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

}