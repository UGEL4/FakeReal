#include "Shader_GL.h"
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <assert.h>

namespace FakeReal {

	static GLenum ShaderTypeFromString(const std::string& typeName)
	{
		if (typeName == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (typeName == "fragment" || typeName == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}
		assert(0);
		return 0;
	}

	Shader_GL::Shader_GL(const std::string_view& filePath)
		:m_FilePath(filePath)
	{
		std::string sorce = ReadFile(filePath);
		PraseShader(sorce);
		CreateShader();

		SetShaderName();
		std::cout << "¹¹ÔìOpenGLShader" << std::endl;
	}

	Shader_GL::Shader_GL()
	{

	}

	Shader_GL::~Shader_GL()
	{
		std::cout << "Ïú»ÙOpenGLShader" << std::endl;
	}

	void Shader_GL::Bind() const
	{
		glUseProgram(m_RandererID);
	}

	void Shader_GL::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader_GL::SetMatrix4(const char* name, const float* mat, bool transpose /*= false*/) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniformMatrix4fv(location, 1, transpose, mat);
	}

	void Shader_GL::SetMatrix4Array(const char* name, int count, const float* mat, bool transpose /*= false*/) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniformMatrix4fv(location, count, transpose, mat);
	}

	void Shader_GL::SetMatrix3(const char* name, const float* mat, bool transpose /*= false*/) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniformMatrix3fv(location, 1, transpose, mat);
	}

	void Shader_GL::SetMatrix3Array(const char* name, int count, const float* mat, bool transpose /*= false*/) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniformMatrix3fv(location, count, transpose, mat);
	}

	void Shader_GL::SetFloat3(const char* name, const float* val) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniform3fv(location, 1, val);
	}

	void Shader_GL::SetFloat3(const char* name, float x, float y, float z) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniform3f(location, x, y, z);
	}

	void Shader_GL::SetFloat4(const char* name, const float* val) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniform4fv(location, 1, val);
	}

	void Shader_GL::SetFloat4(const char* name, float x, float y, float z, float w) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniform4f(location, x, y, z, w);
	}

	void Shader_GL::SetFloat2(const char* name, const float* val) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniform2fv(location, 1, val);
	}

	void Shader_GL::SetFloat2(const char* name, float x, float y) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniform2f(location, x, y);
	}

	void Shader_GL::SetFloat(const char* name, float val) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniform1f(location, val);
	}

	void Shader_GL::SetInt(const char* name, int val) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniform1i(location, val);
	}

	void Shader_GL::SetIntArray(const char* name, const int* val, uint32_t count) const
	{
		GLint location = glGetUniformLocation(m_RandererID, name);
		glUniform1iv(location, count, val);
	}

	void Shader_GL::SetShaderFilePath(const std::string_view& path)
	{
		m_FilePath = path;
	}

	bool Shader_GL::CreateFromFile()
	{
		std::string sorce = ReadFile(m_FilePath);
		PraseShader(sorce);
		CreateShader();

		SetShaderName();
		return true;
	}

	bool Shader_GL::CreateFromFile(const std::string_view& file)
	{
		m_FilePath = file;
		std::string sorce = ReadFile(m_FilePath);
		PraseShader(sorce);
		CreateShader();

		SetShaderName();
		return true;
	}

	std::string Shader_GL::ReadFile(const std::string_view& filePath)
	{
		std::string source;
		std::ifstream sorceFile(filePath.data(), std::ios::in | std::ios::binary);
		if (sorceFile)
		{
			sorceFile.seekg(0, std::ios::end);
			size_t size = sorceFile.tellg();
			if (size != -1)
			{
				source.resize(size);
				sorceFile.seekg(0, std::ios::beg);
				sorceFile.read(&source[0], size);
			}
			else
			{
				std::cout << "Could not read from file " << filePath.data() << std::endl;
				assert(0);
			}
		}
		else
		{
			std::cout <<"shader file open failed!" << std::endl;
			assert(0);
		}
		return source;
	}

	void Shader_GL::PraseShader(const std::string& source)
	{
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			assert(eol != std::string::npos);
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			assert(ShaderTypeFromString(type));

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			assert(nextLinePos != std::string::npos);
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			m_ShaderSorceCode[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
	}

	void Shader_GL::CreateShader()
	{
		m_RandererID = glCreateProgram();
		uint32_t vs = CompileShader(GL_VERTEX_SHADER, m_ShaderSorceCode[GL_VERTEX_SHADER]);
		uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, m_ShaderSorceCode[GL_FRAGMENT_SHADER]);
		glAttachShader(m_RandererID, vs);
		glAttachShader(m_RandererID, fs);
		glLinkProgram(m_RandererID);
		glValidateProgram(m_RandererID);
		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	uint32_t Shader_GL::CompileShader(GLenum shaderType, const std::string& code)
	{
		uint32_t id = glCreateShader(shaderType);
		const char* source = code.data();
		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);
		assert(IsShaderCompiled(id));
		return id;
	}

	bool Shader_GL::IsShaderCompiled(uint32_t program)
	{
		int result;
		glGetShaderiv(program, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(program, length, &length, message);
			std::cout << message << std::endl;
			return false;
		}

		return true;
	}

	void Shader_GL::SetShaderName()
	{
		// Extract name from filepath
		auto lastSlash = m_FilePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = m_FilePath.rfind('.');
		auto count = lastDot == std::string::npos ? m_FilePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = m_FilePath.substr(lastSlash, count);
	}

}