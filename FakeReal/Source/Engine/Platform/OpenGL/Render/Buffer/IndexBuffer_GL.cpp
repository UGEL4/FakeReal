#include "IndexBuffer_GL.h"
#include <glad/glad.h>
#include <iostream>

namespace FakeReal {
	IndexBuffer_GL::IndexBuffer_GL(const void* data, uint32_t count)
		:m_uiCount(count)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
		std::cout << "构造OpenGLIndexBuffer" << std::endl;
	}

	IndexBuffer_GL::IndexBuffer_GL()
		: m_uiCount(0)
	{
		glGenBuffers(1, &m_RendererID);
		std::cout << "无参构造OpenGLIndexBuffer" << std::endl;
	}

	IndexBuffer_GL::~IndexBuffer_GL()
	{
		glDeleteBuffers(1, &m_RendererID);
		std::cout << "销毁OpenGLIndexBuffer" << std::endl;
	}

	void IndexBuffer_GL::SetData(const void* data, uint32_t count)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
		m_uiCount = count;
	}

	void IndexBuffer_GL::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer_GL::Unind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}