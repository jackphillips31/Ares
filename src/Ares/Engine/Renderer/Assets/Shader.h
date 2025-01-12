#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "Engine/Data/Asset.h"

namespace Ares {

	struct ParsedShaderData;

	class Shader : public AssetBase
	{
	public:
		virtual ~Shader() = default;

		// RendererID access (for low-level operations)
		virtual uint32_t GetRendererID() const = 0;

		// Equality operator
		inline bool operator==(const Shader& other) const
		{
			return GetRendererID() == other.GetRendererID();
		};
	};

	class VertexShader : public Shader
	{
	public:
		virtual ~VertexShader() = default;

	private:
		// Only Asset Manager can create Shaders
		friend class AssetManager;
		static Scope<VertexShader> Create(const std::string& name, const std::string_view shaderSource);
	};

	class FragmentShader : public Shader
	{
	public:
		virtual ~FragmentShader() = default;

	private:
		// Only Asset Manager can create Shaders
		friend class AssetManager;
		static Scope<FragmentShader> Create(const std::string& name, const std::string_view shaderSource);
	};

	class ShaderProgram : public AssetBase
	{
	public:
		virtual ~ShaderProgram() = default;

		// Binding and state
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// Uniform setters
		virtual void SetInt(const std::string& name, const int32_t value) = 0;
		virtual void SetIntArray(const std::string& name, const int32_t* values, const uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, const float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;

		// RendererID access (for low-level operations)
		virtual uint32_t GetRendererID() const = 0;

		// Equality operator
		bool operator==(const ShaderProgram& other) const
		{
			return GetRendererID() == other.GetRendererID();
		}

	private:
		// Only Asset Manager can create Shaders
		friend class AssetManager;
		static Scope<ShaderProgram> Create(const std::string& name, const std::vector<Shader*>& shaders);
		static Scope<ShaderProgram> Create(const std::string& name, const Ref<ParsedShaderData>& shaderData);
	};

}