#pragma once
#include "Engine/Containers/Atomic.h"
#include "Engine/Containers/HashMap.h"
#include "Engine/Data/Asset.h"

namespace Ares {

	class VertexBuffer;
	class IndexBuffer;
	enum class VertexDataType : uint8_t;
	struct ParsedMeshData;

	namespace Systems {

		class AssetManager;

	}

	class MeshData : public AssetBase
	{
	public:
		// Core property
		inline const String& GetName() const override { return m_Name; }

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
		template <typename ObjectType, typename... Args>
		friend Scope<ObjectType> Ares::CreateScope(Args&&... args);
		friend class Systems::AssetManager;
		MeshData(const String& name, const Ref<ParsedMeshData>& meshData);
		static Scope<MeshData> Create(const String& name, const Ref<ParsedMeshData>& meshData);

	private:
		inline static Atomic<uint32_t> s_NextMeshDataId{ 1 };
		mutable std::shared_mutex m_Mutex;
		String m_Name;
		uint32_t m_RendererID;
		HashMap<VertexDataType, Scope<VertexBuffer>> m_VertexBuffers;
		Scope<IndexBuffer> m_IndexBuffer;
	};

}