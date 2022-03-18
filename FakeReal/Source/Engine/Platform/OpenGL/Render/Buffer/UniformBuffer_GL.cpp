#include "UniformBuffer_GL.h"
#include <glad/glad.h>
#include<iostream>
namespace FakeReal {
	UniformBuffer_GL::UniformBuffer_GL(unsigned int uiSize, unsigned int uiBinding)
	{
		Initial(uiSize, uiBinding);
		std::cout << "构造OpenGLUniformBuffer" << std::endl;
	}

	UniformBuffer_GL::UniformBuffer_GL()
	{
		std::cout << "无参构造OpenGLUniformBuffer" << std::endl;
	}

	UniformBuffer_GL::~UniformBuffer_GL()
	{
		glDeleteBuffers(1, &m_RendererID);
		std::cout << "销毁OpenGLUniformBuffer" << std::endl;
	}

	void UniformBuffer_GL::SetData(const void* pData, unsigned int uiSize, unsigned int uiOffset /*= 0*/)
	{
		//glNamedBufferSubData(m_RendererID, uiOffset, uiSize, pData);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, uiOffset, uiSize, pData);
	}

	void UniformBuffer_GL::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	}

	void UniformBuffer_GL::Unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void UniformBuffer_GL::Initial(unsigned int uiSize, unsigned int uiBinding)
	{
		//glCreateBuffers(1, &m_RendererID);
		glGenBuffers(1, &m_RendererID);
		//glNamedBufferData(m_RendererID, uiSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferData(GL_UNIFORM_BUFFER, uiSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, uiBinding, m_RendererID);
	}

}
