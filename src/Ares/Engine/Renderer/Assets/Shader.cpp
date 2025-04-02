#include <arespch.h>
#include "Engine/Renderer/Assets/Shader.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Ares {

	Scope<VertexShader> VertexShader::Create(const StringView name, const StringView shaderSource)
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::None:	AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL:	return CreateScope<OpenGLVertexShader>(name, shaderSource);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Scope<FragmentShader> FragmentShader::Create(const StringView name, const StringView shaderSource)
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::None:	AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL:	return CreateScope<OpenGLFragmentShader>(name, shaderSource);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Scope<ShaderProgram> ShaderProgram::Create(const StringView name, const Vector<Shader*>& shaders)
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::None:	AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL:	return CreateScope<OpenGLShaderProgram>(name, shaders);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Scope<ShaderProgram> ShaderProgram::Create(const StringView name, const Ref<ParsedShaderData>& shaderData)
	{
		switch (Systems::Renderer::GetAPI())
		{
		case RenderAPI::None:	AR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL:	return CreateScope<OpenGLShaderProgram>(name, shaderData);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}