#include "Shader.h"
#include <iostream>
#include <assert.h>
#include "../../Platform/OpenGL/Render/Shader/Shader_GL.h"

namespace FakeReal {

	Shader* Shader::Create(const std::string_view& filePath)
	{
		return new Shader_GL(filePath);
	}

	std::vector<std::string_view> ShaderLibrary::ms_DafaultShaderPath = {
		"Shader/OpenGL/BaseShader.glshader",
		"Shader/OpenGL/SkyBox.glshader",
		"Shader/OpenGL/Border.glshader"
	};
	ShaderLibrary* ShaderLibrary::gs_pShaderLib= new ShaderLibrary();

	ShaderLibrary::~ShaderLibrary()
	{
		for (auto& p : m_shader_map)
		{
			if (p.second != nullptr) delete p.second;
		}
	}

	void ShaderLibrary::Add(Shader* pShader)
	{
		if (IsExist(pShader->GetName()))
		{
			std::cout << "shader exists" << std::endl;
			assert(0);
		}
		m_shader_map[pShader->GetName()] = pShader;
	}

	void ShaderLibrary::LoadShader(const std::string_view& filePath)
	{
		Shader* pShader = Shader::Create(filePath);
		Add(pShader);
	}

	Shader* ShaderLibrary::Get(const std::string_view& name)
	{
		if (IsExist(name))
		{
			return m_shader_map[name];
		}
		assert(0);
		return nullptr;
	}

	bool ShaderLibrary::IsExist(const std::string_view& name) const
	{
		return m_shader_map.find(name) != m_shader_map.end();
	}

	void ShaderLibrary::LoadDefaultShader()
	{
		for (auto& str : ms_DafaultShaderPath)
		{
			LoadShader(str);
		}
	}

}
