#pragma once

#include "Engine/Core/Core.h"

namespace Ares {

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Clear(float r, float g, float b, float a) const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uintptr_t GetColorAttachmentHandle() const = 0;

		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);
	};

}