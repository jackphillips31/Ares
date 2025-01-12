#include <arespch.h>
#include "Engine/ECS/Components/Mesh.h"

#include "Engine/Data/AssetManager.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/BufferLayout.h"
#include "Engine/Renderer/Assets/MeshData.h"

namespace Ares::ECS::Components {

	Mesh::Mesh(const Ref<Asset>& asset)
	{
		if (asset->GetType() != typeid(MeshData))
		{
			AR_CORE_ASSERT(false, "Asset must be Mesh Data!");
			return;
		}
		m_MeshAsset = asset;
	}

	VertexBuffer* Mesh::GetPositionBuffer() const
	{
		return GetBuffer(VertexDataType::Position);
	}

	VertexBuffer* Mesh::GetTextureBuffer() const
	{
		return GetBuffer(VertexDataType::TextureCoords);
	}

	VertexBuffer* Mesh::GetNormalBuffer() const
	{
		return GetBuffer(VertexDataType::Normal);
	}

	IndexBuffer* Mesh::GetIndexBuffer() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_MeshAsset != nullptr && m_MeshAsset->GetState() == AssetState::Loaded)
			return m_MeshAsset->GetAsset<MeshData>()->GetIndexBuffer();

		return nullptr;
	}

	std::string Mesh::GetMeshName() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_MeshAsset != nullptr)
			return m_MeshAsset->GetName();

		return "NULL";
	}

	size_t Mesh::GetMeshSize() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_MeshAsset != nullptr)
			return m_MeshAsset->GetDataSize();

		return 0;
	}

	bool Mesh::IsLoaded() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_MeshAsset != nullptr && m_MeshAsset->GetState() == AssetState::Loaded)
			return true;

		return false;
	}

	bool Mesh::IsValid() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_MeshAsset != nullptr)
			return true;

		return false;
	}

	void Mesh::PreCache() const
	{
		std::shared_lock lock(m_Mutex);
		if (m_MeshAsset != nullptr && m_MeshAsset->GetState() == AssetState::Staged)
			AssetManager::Load(m_MeshAsset);
	}

	VertexBuffer* Mesh::GetBuffer(VertexDataType type) const
	{
		std::shared_lock lock(m_Mutex);
		if (m_MeshAsset != nullptr && m_MeshAsset->GetState() == AssetState::Loaded)
		{
			return m_MeshAsset->GetAsset<MeshData>()->GetVertexBuffer(type);
		}
		return nullptr;
	}

}