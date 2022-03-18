#pragma once

#include "../../../../Render/Shader/Shader.h"
#include "../../../../Core/CoreMarco.h"
#include <glad/glad.h>
namespace FakeReal {
	class FR_ENGINE_API Shader_GL : public Shader
	{
	public:
		Shader_GL();
		Shader_GL(const std::string_view& filePath);
		~Shader_GL();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual const std::string_view& GetName() const override { return m_Name; }

		virtual void SetMatrix4(const char* name, const float* mat, bool transpose = false) const override;
		virtual void SetMatrix4Array(const char* name, int count, const float* mat, bool transpose = false) const override;
		virtual void SetMatrix3(const char* name, const float* mat, bool transpose = false) const override;
		virtual void SetMatrix3Array(const char* name, int count, const float* mat, bool transpose = false) const override;
		virtual void SetFloat4(const char* name, const float* val) const override;
		virtual void SetFloat4(const char* name, float x, float y, float z, float w) const override;
		virtual void SetFloat3(const char* name, const float* val) const override;
		virtual void SetFloat3(const char* name, float x, float y, float z) const override;
		virtual void SetFloat2(const char* name, const float* val) const override;
		virtual void SetFloat2(const char* name, float x, float y) const override;
		virtual void SetFloat(const char* name, float val) const override;
		virtual void SetInt(const char* name, int val) const override;
		virtual void SetIntArray(const char* name, const int* val, uint32_t count) const override;
		virtual void SetShaderFilePath(const std::string_view& path) override;

	public:
		bool CreateFromFile();
		bool CreateFromFile(const std::string_view& file);
	private:
		std::string ReadFile(const std::string_view& filePath);
		void PraseShader(const std::string& source);
		void CreateShader();
		uint32_t CompileShader(GLenum shaderType, const std::string& code);
		bool IsShaderCompiled(uint32_t program);
		void SetShaderName();
	private:
		uint32_t m_RandererID;
		std::string_view m_FilePath;
		std::string_view m_Name;
		std::unordered_map<GLenum, std::string> m_ShaderSorceCode;
	};

}