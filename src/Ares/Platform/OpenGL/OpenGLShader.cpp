#include <arespch.h>
#include <fstream>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

namespace Ares {

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		std::unordered_map<uint32_t, std::string> shaderSource = PreProcess(source);
		Compile(shaderSource);

		// Extract name from filepath
		size_t lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		size_t lastDot = filepath.rfind(".");
		size_t count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<uint32_t, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& shaderSource)
		: m_Name(name)
	{
		Compile(PreProcess(shaderSource));
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	uint32_t OpenGLShader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		AR_CORE_ERROR("Shader: {}", type);
		AR_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				AR_CORE_ERROR("Could not read from file: '{}'", filepath);
			}
		}
		else
		{
			AR_CORE_ERROR("Could not open file: '{}'", filepath);
		}

		return result;
	}

	std::unordered_map<uint32_t, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<uint32_t, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);

		// Start of shader type declaration line
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			// End of shader type declaration line
			size_t eol = source.find_first_of("\r\n", pos);
			AR_CORE_ASSERT(eol != std::string::npos, "Shader Syntax Error!");

			// Start of shader type name (after "#type " keyword)
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			AR_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specification!");

			// Start of shader code after shader type declaration line
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			AR_CORE_ASSERT(nextLinePos != std::string::npos, "Shader Syntax Error!");

			// Start of next shader type declaration line
			pos = source.find(typeToken, nextLinePos);

			shaderSources[ShaderTypeFromString(type)] =
				(pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<uint32_t, std::string>& shaderSources)
	{
		uint32_t program = glCreateProgram();
		AR_CORE_ASSERT(shaderSources.size() <= 2, "Only support maximum of 2 shaders.");
		std::array<uint32_t, 2> glShaderIDs;
		int32_t glShaderIDIndex = 0;

		for (auto& keyValue : shaderSources)
		{
			uint32_t shaderType = keyValue.first;
			const std::string& shaderSource = keyValue.second;

			uint32_t shader = glCreateShader(shaderType);

			const char* sourceCStr = shaderSource.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			int32_t isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				int32_t maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				AR_CORE_ERROR("Shader Compile Error: {}", infoLog.data());
				AR_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		int32_t isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int32_t maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			AR_CORE_ERROR("Shader Program Error: {}", infoLog.data());
			AR_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int32_t value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int32_t* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values)
	{
		UploadUniformFloat2(name, values);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values)
	{
		UploadUniformFloat3(name, values);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values)
	{
		UploadUniformFloat4(name, values);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		UploadUniformMat3(name, matrix);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		UploadUniformMat4(name, matrix);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int32_t value)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int32_t* values, uint32_t count)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}