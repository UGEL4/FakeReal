#include "FrameBuffer.h"
#include "../Renderer.h"
#include "../../Platform/OpenGL/Render/Buffer/FrameBuffer_GL.h"

namespace FakeReal {

	FrameBuffer::FrameBuffer()
	{

	}

	FrameBuffer::~FrameBuffer()
	{

	}

	FrameBuffer* FrameBuffer::Create(unsigned int uiWidth, unsigned int uiHeight)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::OpenGL: return new FrameBuffer_GL(uiWidth, uiHeight);
		}
		return nullptr;
	}

}
