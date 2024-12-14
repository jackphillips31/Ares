#include <arespch.h>

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Engine/Renderer/Shader.h"

namespace Ares {

	Ref<VertexShader> VertexShader::Create(const std::string& name, const FileBuffer& fileBuffer)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexShader>(name, fileBuffer);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexShader> VertexShader::Create(const std::string& name, const std::string& shaderSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexShader>(name, shaderSource);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<FragmentShader> FragmentShader::Create(const std::string& name, const FileBuffer& fileBuffer)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFragmentShader>(name, fileBuffer);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<FragmentShader> FragmentShader::Create(const std::string& name, const std::string& shaderSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLFragmentShader>(name, shaderSource);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<ShaderProgram> ShaderProgram::Create(const std::string& name, const std::vector<Ref<Shader>>& shaders)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	AR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:	return CreateRef<OpenGLShaderProgram>(name, shaders);
		}

		AR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}