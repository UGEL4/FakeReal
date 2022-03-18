#pragma once
#include <unordered_map>
#include <vector>
#include <string_view>
#include "../../Core/CoreMarco.h"
namespace FakeReal {
	class FR_ENGINE_API Shader
	{
	public:
		Shader() {}
		virtual ~Shader() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const std::string_view& GetName() const = 0;

		virtual void SetMatrix4(const char* name, const float* mat, bool transpose = false) const = 0;
		virtual void SetMatrix4Array(const char* name, int count, const float* mat, bool transpose = false) const = 0;
		virtual void SetMatrix3(const char* name, const float* mat, bool transpose = false) const  = 0;
		virtual void SetMatrix3Array(const char* name, int count, const float* mat, bool transpose = false) const  = 0;
		virtual void SetFloat3(const char* name, const float* val) const = 0;
		virtual void SetFloat3(const char* name, float x, float y, float z) const = 0;
		virtual void SetFloat4(const char* name, const float* val) const = 0;
		virtual void SetFloat4(const char* name, float x, float y, float z, float w) const = 0;
		virtual void SetFloat2(const char* name, const float* val) const = 0;
		virtual void SetFloat2(const char* name, float x, float y) const = 0;
		virtual void SetFloat(const char* name, float val) const = 0;
		virtual void SetInt(const char* name, int val) const = 0;
		virtual void SetIntArray(const char* name, const int* val, uint32_t count) const = 0;
		virtual void SetShaderFilePath(const std::string_view& path) = 0;

	public:
		static Shader* Create(const std::string_view& filePath);
	};

	class FR_ENGINE_API ShaderLibrary
	{
	public:
		~ShaderLibrary();
		void Add(Shader* pShader);
		void LoadShader(const std::string_view& filePath);
		Shader* Get(const std::string_view& name);
		bool IsExist(const std::string_view& name) const;
		void LoadDefaultShader();
	public:
		static ShaderLibrary* gs_pShaderLib;
	private:
		std::unordered_map<std::string_view, Shader*> m_shader_map;
		static std::vector<std::string_view> ms_DafaultShaderPath;
	};

}
