#pragma once

#include "Engine/Core/Core.h"

namespace Ares {

	class SharedGraphicsContext
	{
	public:
		virtual ~SharedGraphicsContext() = default;

		// Makes the context current on the calling thread
		virtual void MakeCurrent() = 0;

		// Detaches the context from the calling thread
		virtual void DetachCurrent() = 0;

		static Scope<SharedGraphicsContext> Create(const void* primaryContext);
	};

}