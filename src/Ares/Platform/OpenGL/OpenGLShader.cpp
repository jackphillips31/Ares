#include <arespch.h>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

namespace Ares {

	OpenGLVertexShader::OpenGLVertexShader(const std::string& name, const FileBuffer& fileBuffer)
		: m_Name(name), m_ShaderId(0)
	{
		int32_t shaderSize = static_cast<int32_t>(fileBuffer.GetSize());
		if (shaderSize > static_cast<int32_t>(std::numeric_limits<GLint>::max()))
		{
			AR_CORE_ASSERT(false, "Shader source size exceeds GLint maximum value!");
			return;
		}

		Compile(static_cast<const char*>(fileBuffer.GetBuffer()), shaderSize);
	}

	OpenGLVertexShader::OpenGLVertexShader(const std::string& name, const std::string& shaderSource)
		: m_Name(name), m_ShaderId(0)
	{
		int32_t shaderSize = static_cast<int32_t>(shaderSource.size() * sizeof(char));
		if (shaderSize > static_cast<int32_t>(std::numeric_limits<GLint>::max()))
		{
			AR_CORE_ASSERT(false, "Shader source size exceeds GLint maximum value!");
			return;
		}

		Compile(shaderSource.c_str(), shaderSize);
	}

	OpenGLVertexShader::~OpenGLVertexShader()
	{
		if (m_ShaderId != 0)
		{
			glDeleteShader(m_ShaderId);
			m_ShaderId = 0;
		}
	}

	uint32_t OpenGLVertexShader::GetShaderType() const
	{
		return GL_VERTEX_SHADER;
	}

	void OpenGLVertexShader::Compile(const char* shaderSource, const int32_t shaderSize)
	{
		m_ShaderId = glCreateShader(GetShaderType());

		glShaderSource(m_ShaderId, 1, static_cast<const GLchar *const*>(&shaderSource), static_cast<const GLint*>(&shaderSize));
		glCompileShader(m_ShaderId);

		GLint isCompiled = 0;
		glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(m_ShaderId, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(m_ShaderId, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(m_ShaderId);

			AR_CORE_ERROR("Shader Compilation Error: {}", static_cast<char*>(infoLog.data()));
			AR_CORE_ASSERT(false, "Shader Compilation Failure!");
			return;
		}
	}

	OpenGLFragmentShader::OpenGLFragmentShader(const std::string& name, const FileBuffer& fileBuffer)
		: m_Name(name), m_ShaderId(0)
	{
		int32_t shaderSize = static_cast<int32_t>(fileBuffer.GetSize());
		if (shaderSize > static_cast<int32_t>(std::numeric_limits<GLint>::max()))
		{
			AR_CORE_ASSERT(false, "Shader source size exceeds GLint maximum value!");
			return;
		}

		Compile(static_cast<const char*>(fileBuffer.GetBuffer()), shaderSize);
	}

	OpenGLFragmentShader::OpenGLFragmentShader(const std::string& name, const std::string& shaderSource)
		: m_Name(name), m_ShaderId(0)
	{
		int32_t shaderSize = static_cast<int32_t>(shaderSource.size() * sizeof(char));
		if (shaderSize > static_cast<int32_t>(std::numeric_limits<GLint>::max()))
		{
			AR_CORE_ASSERT(false, "Shader source size exceeds GLint maximum value!");
			return;
		}

		Compile(shaderSource.c_str(), shaderSize);
	}

	OpenGLFragmentShader::~OpenGLFragmentShader()
	{
		if (m_ShaderId != 0)
		{
			glDeleteShader(m_ShaderId);
			m_ShaderId = 0;
		}
	}

	uint32_t OpenGLFragmentShader::GetShaderType() const
	{
		return GL_FRAGMENT_SHADER;
	}

	void OpenGLFragmentShader::Compile(const char* shaderSource, const int32_t shaderSize)
	{
		m_ShaderId = glCreateShader(GetShaderType());

		glShaderSource(m_ShaderId, 1, static_cast<const GLchar* const*>(&shaderSource), static_cast<const GLint*>(&shaderSize));
		glCompileShader(m_ShaderId);

		GLint isCompiled = 0;
		glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(m_ShaderId, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(m_ShaderId, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(m_ShaderId);

			AR_CORE_ERROR("Shader Compilation Error: {}", static_cast<char*>(infoLog.data()));
			AR_CORE_ASSERT(false, "Shader Compilation Failure!");
			return;
		}
	}

	OpenGLShaderProgram::OpenGLShaderProgram(const std::string& name, const std::vector<Ref<Shader>>& shaders)
		: m_Name(name)
	{
		LinkShaders(shaders);
	}

	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		if (m_ProgramId != 0)
		{
			glDeleteProgram(m_ProgramId);
			m_ProgramId = 0;
		}
	}

	void OpenGLShaderProgram::LinkShaders(const std::vector<Ref<Shader>>& shaders)
	{
		m_ProgramId = glCreateProgram();

		for (auto& shader : shaders)
		{
			glAttachShader(m_ProgramId, static_cast<GLuint>(shader->GetId()));
		}

		glLinkProgram(m_ProgramId);

		GLint isLinked = 0;
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramId, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_ProgramId);

			AR_CORE_ERROR("Shader Program Error: {}", static_cast<char*>(infoLog.data()));
			AR_CORE_ASSERT(false, "Shader Program Link Failure!");
			return;
		}

		for (auto& shader : shaders)
		{
			glDetachShader(m_ProgramId, shader->GetId());
		}
	}

	void OpenGLShaderProgram::Bind() const
	{
		glUseProgram(m_ProgramId);
	}

	void OpenGLShaderProgram::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShaderProgram::SetInt(const std::string& name, int32_t value)
	{
		UploadUniformInt(name, static_cast<GLint>(value));
	}

	void OpenGLShaderProgram::SetIntArray(const std::string& name, int32_t* values, uint32_t count)
	{
		UploadUniformIntArray(name, static_cast<GLsizei>(count), static_cast<GLint*>(values));
	}

	void OpenGLShaderProgram::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, static_cast<GLfloat>(value));
	}

	void OpenGLShaderProgram::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		UploadUniformFloat2(name, static_cast<GLfloat>(values.x), static_cast<GLfloat>(values.y));
	}

	void OpenGLShaderProgram::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		UploadUniformFloat3(name, static_cast<GLfloat>(values.x), static_cast<GLfloat>(values.y), static_cast<GLfloat>(values.z));
	}

	void OpenGLShaderProgram::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		UploadUniformFloat4(name, static_cast<GLfloat>(values.x), static_cast<GLfloat>(values.y), static_cast<GLfloat>(values.z), static_cast<GLfloat>(values.w));
	}

	void OpenGLShaderProgram::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		UploadUniformMat3(name, static_cast<const GLfloat*>(glm::value_ptr(matrix)));
	}

	void OpenGLShaderProgram::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		UploadUniformMat4(name, static_cast<const GLfloat*>(glm::value_ptr(matrix)));
	}

	void OpenGLShaderProgram::UploadUniformInt(const std::string& name, GLint value)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShaderProgram::UploadUniformIntArray(const std::string& name, GLsizei count, const GLint* values)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShaderProgram::UploadUniformFloat(const std::string& name, GLfloat value)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShaderProgram::UploadUniformFloat2(const std::string& name, GLfloat v0, GLfloat v1)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform2f(location, v0, v1);
	}

	void OpenGLShaderProgram::UploadUniformFloat3(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform3f(location, v0, v1, v2);
	}

	void OpenGLShaderProgram::UploadUniformFloat4(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniform4f(location, v0, v1, v2, v3);
	}

	void OpenGLShaderProgram::UploadUniformMat3(const std::string& name, const GLfloat* values)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, values);
	}

	void OpenGLShaderProgram::UploadUniformMat4(const std::string& name, const GLfloat* values)
	{
		GLint location = glGetUniformLocation(m_ProgramId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, values);
	}

}