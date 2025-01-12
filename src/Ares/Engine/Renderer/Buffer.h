#pragma once

namespace Ares {

	class BufferLayout;
	struct RawData;

	enum class BufferUsage : uint8_t
	{
		Dynamic,
		Static
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		// Core properties
		virtual size_t GetSize() const = 0;
		virtual size_t GetCount() const = 0;
		virtual const BufferLayout& GetBufferLayout() const = 0;

		// Binding and state
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// Setters
		virtual void SetData(const RawData& data, const BufferUsage usage = BufferUsage::Dynamic) = 0;
		virtual void SetSubData(const RawData& data, const uint32_t offset = 0) = 0;
		virtual void SetBufferLayout(const BufferLayout& layout) = 0;

		// RendererID access (for low-level operations)
		virtual uint32_t GetRendererID() const = 0;

		// Creation methods
		static Scope<VertexBuffer> Create(const size_t size, const BufferUsage usage = BufferUsage::Dynamic);
		static Scope<VertexBuffer> Create(const RawData& data, const BufferUsage usage = BufferUsage::Static);

		// Equality operator
		inline bool operator==(const VertexBuffer& other) const
		{
			return GetRendererID() == other.GetRendererID();
		}
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		// Core properties
		virtual size_t GetSize() const = 0;
		virtual size_t GetCount() const = 0;

		// Binding and state
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// Setters
		virtual void SetData(const RawData& data, const BufferUsage usage = BufferUsage::Dynamic) = 0;
		virtual void SetSubData(const RawData& data) = 0;

		// RendererID access (for low-level operations)
		virtual uint32_t GetRendererID() const = 0;

		// Creation methods
		static Scope<IndexBuffer> Create(const size_t size, const BufferUsage usage = BufferUsage::Dynamic);
		static Scope<IndexBuffer> Create(const RawData& data, const BufferUsage usage = BufferUsage::Static);

		// Equality operator
		inline bool operator==(const IndexBuffer& other) const
		{
			return GetRendererID() == other.GetRendererID();
		}
	};

}