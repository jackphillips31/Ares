#include <arespch.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

#include "Engine/Data/Parsers/ShaderParser.h"

namespace Ares {

	GLuint OpenGLShader::Compile(const char* shaderSource, const size_t shaderSize)
	{
		GLint shaderLength = static_cast<GLint>(shaderSize);
		GLuint rendererID = glCreateShader(GetShaderType());

		glShaderSource(rendererID, 1, &shaderSource, &shaderLength);
		glCompileShader(rendererID);

		GLint isCompiled = 0;
		glGetShaderiv(rendererID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(rendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(rendererID, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(rendererID);

			AR_CORE_ERROR("Shader Compilation Error: {}", static_cast<char*>(infoLog.data()));
			AR_CORE_ASSERT(false, "Shader Compilation Failure!");
			return 0;
		}

		return rendererID;
	}

	OpenGLVertexShader::OpenGLVertexShader(const StringView name, const StringView shaderSource)
		: m_Name(name), m_RendererID(0)
	{
		size_t shaderSize = shaderSource.size() * sizeof(char);
		if (shaderSize > std::numeric_limits<GLint>::max())
		{
			AR_CORE_ASSERT(false, "Shader source size exceeds GLint maximum value!");
			return;
		}
		m_RendererID = Compile(shaderSource.data(), shaderSize);
	}

	OpenGLVertexShader::~OpenGLVertexShader()
	{
		if (m_RendererID)
		{
			glDeleteShader(m_RendererID);
			m_RendererID = 0;
		}
	}

	OpenGLFragmentShader::OpenGLFragmentShader(const StringView name, const StringView shaderSource)
		: m_Name(name), m_RendererID(0)
	{
		size_t shaderSize = shaderSource.size() * sizeof(char);
		if (static_cast<GLint>(shaderSize) > std::numeric_limits<GLint>::max())
		{
			AR_CORE_ASSERT(false, "Shader source size exceeds GLint maximum value!");
			return;
		}
		m_RendererID = Compile(shaderSource.data(), shaderSize);
	}

	OpenGLFragmentShader::~OpenGLFragmentShader()
	{
		if (m_RendererID)
		{
			glDeleteShader(m_RendererID);
			m_RendererID = 0;
		}
	}

	OpenGLShaderProgram::OpenGLShaderProgram(const StringView name, const Vector<Shader*>& shaders)
		: m_Name(name), m_RendererID(0)
	{
		LinkShaders(shaders);
	}

	OpenGLShaderProgram::OpenGLShaderProgram(const StringView name, const Ref<ParsedShaderData>& shaderData)
		: m_Name(name), m_RendererID(0)
	{
		OpenGLVertexShader vertexShader = OpenGLVertexShader(String(name.data()) + "_Vertex", shaderData->VertexSource.c_str());
		OpenGLFragmentShader fragmentShader = OpenGLFragmentShader(String(name.data()) + "_FragmentShader", shaderData->FragmentSource.c_str());
		LinkShaders({ &vertexShader, &fragmentShader });
	}

	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		if (m_RendererID)
		{
			glDeleteProgram(m_RendererID);
			m_RendererID = 0;
		}
	}

	void OpenGLShaderProgram::LinkShaders(const Vector<Shader*>& shaders)
	{
		m_RendererID = glCreateProgram();

		for (Shader* shader : shaders)
		{
			if (!shader)
			{
				AR_CORE_ASSERT(false, "Shader isn't valid!");
				return;
			}
			glAttachShader(m_RendererID, static_cast<GLuint>(shader->GetRendererID()));
		}

		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_RendererID);

			AR_CORE_ERROR("Shader Program Error: {}", static_cast<char*>(infoLog.data()));
			AR_CORE_ASSERT(false, "Shader Program Link Failure!");
			return;
		}

		for (Shader* shader : shaders)
		{
			glDetachShader(m_RendererID, shader->GetRendererID());
		}
	}

	void OpenGLShaderProgram::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShaderProgram::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShaderProgram::SetInt(const StringView name, const int32_t value)
	{
		UploadUniformInt(name, static_cast<GLint>(value));
	}

	void OpenGLShaderProgram::SetIntArray(const StringView name, const int32_t* values, const uint32_t count)
	{
		UploadUniformIntArray(name, static_cast<const GLint*>(values), static_cast<GLsizei>(count));
	}

	void OpenGLShaderProgram::SetFloat(const StringView name, const float value)
	{
		UploadUniformFloat(name, static_cast<GLfloat>(value));
	}

	void OpenGLShaderProgram::SetFloat2(const StringView name, const glm::vec2& values)
	{
		UploadUniformFloat2(name, static_cast<GLfloat>(values.x), static_cast<GLfloat>(values.y));
	}

	void OpenGLShaderProgram::SetFloat3(const StringView name, const glm::vec3& values)
	{
		UploadUniformFloat3(name, static_cast<GLfloat>(values.x), static_cast<GLfloat>(values.y), static_cast<GLfloat>(values.z));
	}

	void OpenGLShaderProgram::SetFloat4(const StringView name, const glm::vec4& values)
	{
		UploadUniformFloat4(name, static_cast<GLfloat>(values.x), static_cast<GLfloat>(values.y), static_cast<GLfloat>(values.z), static_cast<GLfloat>(values.w));
	}

	void OpenGLShaderProgram::SetMat3(const StringView name, const glm::mat3& matrix)
	{
		UploadUniformMat3(name, static_cast<const GLfloat*>(glm::value_ptr(matrix)));
	}

	void OpenGLShaderProgram::SetMat4(const StringView name, const glm::mat4& matrix)
	{
		UploadUniformMat4(name, static_cast<const GLfloat*>(glm::value_ptr(matrix)));
	}

	void OpenGLShaderProgram::UploadUniformInt(const StringView name, GLint value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.data());
		glUniform1i(location, value);
	}

	void OpenGLShaderProgram::UploadUniformIntArray(const StringView name, const GLint* values, const GLsizei count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.data());
		glUniform1iv(location, count, values);
	}

	void OpenGLShaderProgram::UploadUniformFloat(const StringView name, GLfloat value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.data());
		glUniform1f(location, value);
	}

	void OpenGLShaderProgram::UploadUniformFloat2(const StringView name, GLfloat v0, GLfloat v1)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.data());
		glUniform2f(location, v0, v1);
	}

	void OpenGLShaderProgram::UploadUniformFloat3(const StringView name, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.data());
		glUniform3f(location, v0, v1, v2);
	}

	void OpenGLShaderProgram::UploadUniformFloat4(const StringView name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.data());
		glUniform4f(location, v0, v1, v2, v3);
	}

	void OpenGLShaderProgram::UploadUniformMat3(const StringView name, const GLfloat* values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.data());
		glUniformMatrix3fv(location, 1, GL_FALSE, values);
	}

	void OpenGLShaderProgram::UploadUniformMat4(const StringView name, const GLfloat* values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.data());
		glUniformMatrix4fv(location, 1, GL_FALSE, values);
	}

}