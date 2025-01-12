#pragma once

namespace Ares {

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void MakeCurrent() = 0;

		virtual void* GetContextHandle() const = 0;

		static Scope<GraphicsContext> Create(void* window);
	};

}