#include "FrameBuffer_GL.h"
#include <glad/glad.h>

namespace FakeReal {

	FrameBuffer_GL::FrameBuffer_GL(unsigned int uiWidth, unsigned int uiHeight)
	{
		m_uiWidth = uiWidth;
		m_uiHeight = uiHeight;
		Invalidata();
	}

	FrameBuffer_GL::~FrameBuffer_GL()
	{
		glDeleteFramebuffers(1, &m_uiRendererID);
		glDeleteTextures(1, &m_uiColorAttachmentID);
		glDeleteTextures(1, &m_uiDepthStencilID);
	}

	void FrameBuffer_GL::Invalidata()
	{
		if (m_uiRendererID)
		{
			glDeleteFramebuffers(1, &m_uiRendererID);
			glDeleteTextures(1, &m_uiColorAttachmentID);
			glDeleteTextures(1, &m_uiDepthStencilID);

			m_uiRendererID = 0;
			m_uiColorAttachmentID = 0;
			m_uiDepthStencilID = 0;
		}
		glGenFramebuffers(1, &m_uiRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_uiRendererID);

		//颜色附件
		glGenTextures(1, &m_uiColorAttachmentID);
		glBindTexture(GL_TEXTURE_2D, m_uiColorAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_uiWidth, m_uiHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiColorAttachmentID, 0);

		//深度\模板附件
		glGenTextures(1, &m_uiDepthStencilID);
		glBindTexture(GL_TEXTURE_2D, m_uiDepthStencilID);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_uiWidth, m_uiHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_uiDepthStencilID, 0);

		//绘制到纹理
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(1, buffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer_GL::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_uiRendererID);
		glViewport(0, 0, m_uiWidth, m_uiHeight);
	}

	void FrameBuffer_GL::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer_GL::Resize(unsigned int uiWidth, unsigned int uiHeight)
	{
		m_uiWidth = uiWidth;
		m_uiHeight = uiHeight;
		Invalidata();
	}

}
