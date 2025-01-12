#pragma once

namespace Ares {

	class VertexBuffer;
	class IndexBuffer;

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		// Core properties
		virtual const std::vector<VertexBuffer*>& GetVertexBuffers() const = 0;
		virtual IndexBuffer* GetIndexBuffer() const = 0;

		// Binding and state
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// Setters
		virtual void AddVertexBuffer(VertexBuffer* vertexBuffer) = 0;
		virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;

		// RendererID access (for low-level operations)
		virtual uint32_t GetRendererID() const = 0;

		// Creation method
		static Ref<VertexArray> Create();

		// Equality operator
		inline bool operator==(const VertexArray& other) const
		{
			return GetRendererID() == other.GetRendererID();
		}
	};
}