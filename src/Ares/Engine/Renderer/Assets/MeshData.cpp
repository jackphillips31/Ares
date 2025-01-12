#include <arespch.h>
#include "Engine/Renderer/Assets/MeshData.h"

#include "Engine/Data/RawData.h"
#include "Engine/Data/Parsers/OBJParser.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/BufferLayout.h"

namespace Ares {

	MeshData::MeshData(const std::string& name, const Ref<ParsedMeshData>& meshData)
		: m_Name(name), m_RendererID(s_NextMeshDataId++)
	{
		m_VertexBuffers[VertexDataType::Position] = VertexBuffer::Create({ meshData->Positions.data(), meshData->Positions.size() * sizeof(float) }, BufferUsage::Static);
		m_VertexBuffers[VertexDataType::Position]->SetBufferLayout({ {VertexDataType::Position} });
		m_VertexBuffers[VertexDataType::TextureCoords] = VertexBuffer::Create({ meshData->TextureCoordinates.data(), meshData->TextureCoordinates.size() * sizeof(float)}, BufferUsage::Static);
		m_VertexBuffers[VertexDataType::TextureCoords]->SetBufferLayout({ {VertexDataType::TextureCoords} });
		m_VertexBuffers[VertexDataType::Normal] = VertexBuffer::Create({ meshData->Normals.data(), meshData->Normals.size() * sizeof(float)}, BufferUsage::Static);
		m_VertexBuffers[VertexDataType::Normal]->SetBufferLayout({ {VertexDataType::Normal} });
		m_IndexBuffer = IndexBuffer::Create({ meshData->Indices.data(), meshData->Indices.size() * sizeof(uint32_t)}, BufferUsage::Static);
	}

	Scope<MeshData> MeshData::Create(const std::string& name, const Ref<ParsedMeshData>& meshData)
	{
		return Scope<MeshData>(new MeshData(name, meshData));
	}

	VertexBuffer* MeshData::GetPositionBuffer() const
	{
		return GetVertexBuffer(VertexDataType::Position);
	}

	VertexBuffer* MeshData::GetTextureBuffer() const
	{
		return GetVertexBuffer(VertexDataType::TextureCoords);
	}

	VertexBuffer* MeshData::GetNormalBuffer() const
	{
		return GetVertexBuffer(VertexDataType::Normal);
	}

	VertexBuffer* MeshData::GetVertexBuffer(const VertexDataType type) const
	{
		std::shared_lock lock(m_Mutex);
		auto it = m_VertexBuffers.find(type);
		if (it != m_VertexBuffers.end())
			return it->second.get();
		return nullptr;
	}

	IndexBuffer* MeshData::GetIndexBuffer() const
	{
		return m_IndexBuffer.get();
	}

}