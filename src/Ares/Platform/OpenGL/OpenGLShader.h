#pragma once
#include <glad/gl.h>

#include "Engine/Renderer/Assets/Shader.h"

namespace Ares {

	class OpenGLShader
	{
	public:
		virtual ~OpenGLShader() = default;

	protected:
		virtual GLenum GetShaderType() const = 0;
		GLuint Compile(const char* shaderSource, const size_t shaderSize);
	};

	class OpenGLVertexShader : public OpenGLShader, public VertexShader
	{
	public:
		OpenGLVertexShader(const std::string& name, const std::string_view shaderSource);
		~OpenGLVertexShader() override;

		// Core property
		inline const std::string& GetName() const override { return m_Name; }

		// Renderer ID access (for low-level operations)
		inline uint32_t GetRendererID() const override { return static_cast<uint32_t>(m_RendererID); }

	private:
		inline GLenum GetShaderType() const override { return GL_VERTEX_SHADER; }

	private:
		std::string m_Name;
		GLuint m_RendererID;
	};

	class OpenGLFragmentShader : public OpenGLShader, public FragmentShader
	{
	public:
		OpenGLFragmentShader(const std::string& name, const std::string_view shaderSource);
		~OpenGLFragmentShader() override;

		// Core property
		inline const std::string& GetName() const override { return m_Name; }

		// Renderer ID access (for low-level operations)
		inline uint32_t GetRendererID() const override { return static_cast<uint32_t>(m_RendererID); }

	private:
		inline GLenum GetShaderType() const override { return GL_FRAGMENT_SHADER; }

	private:
		std::string m_Name;
		GLuint m_RendererID;
	};

	class OpenGLShaderProgram : public ShaderProgram
	{
	public:
		OpenGLShaderProgram(const std::string& name, const std::vector<Shader*>& shaders);
		OpenGLShaderProgram(const std::string& name, const Ref<ParsedShaderData>& shaderData);
		~OpenGLShaderProgram() override;
		
		// Core property
		inline const std::string& GetName() const override { return m_Name; }

		// Binding and state
		void Bind() const override;
		void Unbind() const override;

		// Uniform setters
		void SetInt(const std::string& name, const int32_t value) override;
		void SetIntArray(const std::string& name, const int32_t* values, const uint32_t count) override;
		void SetFloat(const std::string& name, const float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& values) override;
		void SetFloat3(const std::string& name, const glm::vec3& values) override;
		void SetFloat4(const std::string& name, const glm::vec4& values) override;
		void SetMat3(const std::string& name, const glm::mat3& matrix) override;
		void SetMat4(const std::string& name, const glm::mat4& matrix) override;

		// Renderer ID access (for low-level operations)
		inline uint32_t GetRendererID() const override { return static_cast<uint32_t>(m_RendererID); }

	private:
		// Utilities
		void LinkShaders(const std::vector<Shader*>& shaders);
		void UploadUniformInt(const std::string& name, const GLint value);
		void UploadUniformIntArray(const std::string& name, const GLint* values, const GLsizei count);
		void UploadUniformFloat(const std::string& name, GLfloat value);
		void UploadUniformFloat2(const std::string& name, GLfloat v0, GLfloat v1);
		void UploadUniformFloat3(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2);
		void UploadUniformFloat4(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void UploadUniformMat3(const std::string& name, const GLfloat* values);
		void UploadUniformMat4(const std::string& name, const GLfloat* values);

	private:
		std::string m_Name;
		GLuint m_RendererID;
	};

}