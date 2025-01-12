#pragma once
#include "Engine/ECS/Core/Component.h"

namespace Ares {

	class Asset;
	class VertexBuffer;
	class IndexBuffer;
	enum class VertexDataType : uint8_t;
	
	namespace ECS::Components {

		class Mesh : public Component
		{
		public:
			Mesh(const Ref<Asset>& asset);

			// Getters
			VertexBuffer* GetPositionBuffer() const;
			VertexBuffer* GetTextureBuffer() const;
			VertexBuffer* GetNormalBuffer() const;
			IndexBuffer* GetIndexBuffer() const;
			std::string GetMeshName() const;
			size_t GetMeshSize() const;

			// Asset properties
			bool IsLoaded() const;
			bool IsValid() const;
			void PreCache() const;

		private:
			VertexBuffer* GetBuffer(VertexDataType type) const;

		private:
			// Mutex for thread safety
			mutable std::shared_mutex m_Mutex;

			// Mesh properties
			Ref<Asset> m_MeshAsset = nullptr;

			//Hash
			friend struct std::hash<Mesh>;
		};

	}

}