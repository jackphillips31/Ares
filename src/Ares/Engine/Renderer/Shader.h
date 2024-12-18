#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/Core.h"
#include "Engine/Data/Asset.h"
#include "Engine/Data/FileBuffer.h"

namespace Ares {

	class Shader : public AssetBase
	{
	public:
		virtual ~Shader() = default;

		virtual uint32_t GetId() const = 0;
		virtual const std::string& GetName() const = 0;
	};

	class VertexShader : public Shader
	{
	public:
		virtual ~VertexShader() = default;

		static Ref<VertexShader> Create(const std::string& name, const FileBuffer& fileBuffer);
		static Ref<VertexShader> Create(const std::string& name, const std::string& shaderSource);
	};

	class FragmentShader : public Shader
	{
	public:
		virtual ~FragmentShader() = default;

		static Ref<FragmentShader> Create(const std::string& name, const FileBuffer& fileBuffer);
		static Ref<FragmentShader> Create(const std::string& name, const std::string& shaderSource);
	};

	class ShaderProgram : public AssetBase
	{
	public:
		virtual ~ShaderProgram() = default;

		virtual const std::string& GetName() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int32_t value) = 0;
		virtual void SetIntArray(const std::string& name, int32_t* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;

		static Ref<ShaderProgram> Create(const std::string& name, const std::vector<Ref<Shader>>& shaders);
	};

}