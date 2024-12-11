#pragma once

#include "Engine/Renderer/SharedGraphicsContext.h"

namespace Ares {

	class WinOpenGLSharedContext : public SharedGraphicsContext
	{
	public:
		WinOpenGLSharedContext(const void* primaryContext);
		~WinOpenGLSharedContext() override;

		void MakeCurrent() override;
		void DetachCurrent() override;

	private:
		HDC m_HDC;
		HGLRC m_Context;
	};

}