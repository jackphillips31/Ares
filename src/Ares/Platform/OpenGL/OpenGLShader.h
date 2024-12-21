#pragma once

#include <glad/gl.h>

#include "Engine/Renderer/Shader.h"

namespace Ares {

	class OpenGLVertexShader : public VertexShader
	{
	public:
		OpenGLVertexShader(const std::string& name, const FileBuffer& fileBuffer);
		OpenGLVertexShader(const std::string& name, const std::string& shaderSource);
		virtual ~OpenGLVertexShader();

		inline virtual uint32_t GetId() const override { return m_ShaderId; }
		inline virtual const std::string& GetName() const override { return m_Name; }

	private:
		inline virtual GLenum GetShaderType() const;
		virtual void Compile(const char* shaderSource, const int32_t shaderSize);

	private:
		std::string m_Name;
		GLuint m_ShaderId;
	};

	class OpenGLFragmentShader : public FragmentShader
	{
	public:
		OpenGLFragmentShader(const std::string& name, const FileBuffer& fileBuffer);
		OpenGLFragmentShader(const std::string& name, const std::string& shaderSource);
		virtual ~OpenGLFragmentShader();

		inline virtual uint32_t GetId() const override { return m_ShaderId; }
		inline virtual const std::string& GetName() const override { return m_Name; }

	private:
		inline virtual GLenum GetShaderType() const;
		virtual void Compile(const char* shaderSource, const int32_t shaderSize);

	private:
		std::string m_Name;
		GLuint m_ShaderId;
	};

	class OpenGLShaderProgram : public ShaderProgram
	{
	public:
		OpenGLShaderProgram(const std::string& name, const std::vector<Ref<Shader>>& shaders);
		virtual ~OpenGLShaderProgram();

		inline const std::string& GetName() const override { return m_Name; }

		void Bind() const override;
		void Unbind() const override;

		virtual void SetInt(const std::string& name, int32_t value) override;
		virtual void SetIntArray(const std::string& name, int32_t* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) override;

	private:
		void LinkShaders(const std::vector<Ref<Shader>>& shaders);
		void UploadUniformInt(const std::string& name, GLint value);
		void UploadUniformIntArray(const std::string& name, GLsizei count, const GLint* values);
		void UploadUniformFloat(const std::string& name, GLfloat value);
		void UploadUniformFloat2(const std::string& name, GLfloat v0, GLfloat v1);
		void UploadUniformFloat3(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2);
		void UploadUniformFloat4(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void UploadUniformMat3(const std::string& name, const GLfloat* values);
		void UploadUniformMat4(const std::string& name, const GLfloat* values);

	private:
		std::string m_Name;
		GLuint m_ProgramId;
	};

}