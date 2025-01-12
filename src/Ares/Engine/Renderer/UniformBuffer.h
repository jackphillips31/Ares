#pragma once
#include "Engine/Renderer/Buffer.h"

namespace Ares {

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		// Core properties
		virtual size_t GetSize() const = 0;
		virtual uint32_t GetBindingPoint() const = 0;

		// Binding and state
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// Setters
		virtual void SetData(const RawData& data, const BufferUsage usage = BufferUsage::Dynamic) = 0;
		virtual void SetSubData(const RawData& data, const uint32_t offset = 0) = 0;

		// RendererID access (for low-level operations)
		virtual uint32_t GetRendererID() const = 0;

		// Creation methods
		static Scope<UniformBuffer> Create(const size_t size, const uint32_t bindingPoint, const BufferUsage usage = BufferUsage::Dynamic);
		static Scope<UniformBuffer> Create(const RawData& data, const uint32_t bindingPoint, const BufferUsage usage = BufferUsage::Static);

		// Equality operator
		inline bool operator==(const UniformBuffer& other) const
		{
			return GetRendererID() == other.GetRendererID();
		}
	};

}