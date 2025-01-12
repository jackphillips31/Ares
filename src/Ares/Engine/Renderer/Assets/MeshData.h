#pragma once
#include "Engine/Data/Asset.h"

namespace Ares {

	class AssetManager;
	class VertexBuffer;
	class IndexBuffer;
	enum class VertexDataType : uint8_t;
	struct ParsedMeshData;

	class MeshData : public AssetBase
	{
	public:
		// Core property
		inline const std::string& GetName() const override { return m_Name; }

		// Data getters
		VertexBuffer* GetPositionBuffer() const;
		VertexBuffer* GetTextureBuffer() const;
		VertexBuffer* GetNormalBuffer() const;
		VertexBuffer* GetVertexBuffer(const VertexDataType type) const;
		IndexBuffer* GetIndexBuffer() const;

		// RendererID access (for low-level operations)
		inline uint32_t GetRendererID() const { return m_RendererID; }

		// Equality operator
		inline bool operator==(const MeshData& other) const
		{
			return this->GetRendererID() == other.GetRendererID();
		}

	private:
		// Only Asset Manager can create MeshData
		friend class AssetManager;
		MeshData(const std::string& name, const Ref<ParsedMeshData>& meshData);
		static Scope<MeshData> Create(const std::string& name, const Ref<ParsedMeshData>& meshData);

	private:
		inline static std::atomic<uint32_t> s_NextMeshDataId{ 1 };
		mutable std::shared_mutex m_Mutex;
		std::string m_Name;
		uint32_t m_RendererID;
		std::unordered_map<VertexDataType, Scope<VertexBuffer>> m_VertexBuffers;
		Scope<IndexBuffer> m_IndexBuffer;
	};

}