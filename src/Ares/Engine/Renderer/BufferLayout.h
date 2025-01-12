#pragma once

namespace Ares {

	enum class ShaderDataType : uint8_t
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	enum class VertexDataType : uint8_t
	{
		None = 0,
		Position,
		PositionVec2,
		TextureCoords,
		Normal,
		ColorRGB,
		ColorRGBA,
		Alpha,
		Transform,
		Roughness,
		Metallic,
		Reflectivity,
		EmissiveColor,
		EmissiveIntensity,
		ReceiveShadows,
		CastShadows,
		Wireframe
	};

	static ShaderDataType VertexDataToShaderData(VertexDataType type);
	static size_t ShaderDataTypeSize(ShaderDataType type);
	static size_t UnitDataTypeSize(ShaderDataType type);

	struct BufferElement
	{
		VertexDataType VertexType;
		ShaderDataType DataType;
		size_t Size;
		size_t Offset;
		size_t UnitSize;
		bool Instanced;
		bool Normalized;

		BufferElement(const VertexDataType type, const bool instanced = false, const bool normalized = false);
		uint32_t GetComponentCount() const;

		// Equality operator
		inline bool operator==(const BufferElement& other) const
		{
			return VertexType == other.VertexType && DataType == other.DataType && Size == other.Size && Offset == other.Offset && Instanced == other.Instanced && Normalized == other.Normalized;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout();
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		inline size_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		// Equality operator
		inline bool operator==(const BufferLayout& other) const
		{
			return m_Elements == other.m_Elements;
		}

	private:
		void CalculateOffsetAndStride();

	private:
		std::vector<BufferElement> m_Elements;
		size_t m_Stride = 0;
	};

}