#pragma once

#include "../../../../Render/Buffer/FrameBuffer.h"
#include "../../../../Core/CoreMarco.h"

namespace FakeReal {
	class FR_ENGINE_API FrameBuffer_GL : public FrameBuffer
	{
	public:
		FrameBuffer_GL(unsigned int uiWidth, unsigned int uiHeight);
		~FrameBuffer_GL();
		void Invalidata();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Resize(unsigned int uiWidth, unsigned int uiHeight) override;
		virtual unsigned int GetColorAttachmentRendererID() const override
		{
			return m_uiColorAttachmentID;
		}
		virtual unsigned int GetViewportWidth() const override
		{
			return m_uiWidth;
		}
		virtual unsigned int GetViewportHeight() const override
		{
			return m_uiHeight;
		}

	private:
		unsigned int m_uiRendererID;
		unsigned int m_uiColorAttachmentID;
		unsigned int m_uiDepthStencilID;
		unsigned int m_uiWidth, m_uiHeight;
	};
}
