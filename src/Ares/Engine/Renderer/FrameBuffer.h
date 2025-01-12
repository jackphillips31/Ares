#pragma once

namespace Ares {

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		// Core properties
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Binding and state
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// Utilities
		virtual void Resize(const uint32_t width, const uint32_t height) = 0;

		// TextureID access (for low-level operations)
		virtual uintptr_t GetTextureHandle() const = 0;

		// Creation method
		static Scope<FrameBuffer> Create(const uint32_t width, const uint32_t height);

		// Equality operator
		inline bool operator==(const FrameBuffer& other) const
		{
			return GetTextureHandle() == other.GetTextureHandle();
		}
	};

}