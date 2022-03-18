#pragma once
#include "../../Core/CoreMarco.h"

namespace FakeReal {
	class FR_ENGINE_API FrameBuffer
	{
	public:
		enum class FramebufferTextureFormat
		{
			None = 0,

			// Color
			RGBA8,
			RED_INTEGER,

			// Depth/stencil
			DEPTH24STENCIL8,

			// Defaults
			Depth = DEPTH24STENCIL8
		};
	public:
		FrameBuffer();
		virtual ~FrameBuffer() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Resize(unsigned int uiWidth, unsigned int uiHeight) = 0;
		virtual unsigned int GetColorAttachmentRendererID() const = 0;
		virtual unsigned int GetViewportWidth() const = 0;
		virtual unsigned int GetViewportHeight() const = 0;

		static FrameBuffer* Create(unsigned int uiWidth, unsigned int uiHeight);
	};
}
