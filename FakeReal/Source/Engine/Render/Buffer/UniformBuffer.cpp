#include "UniformBuffer.h"
#include "../Renderer.h"
#include <assert.h>
#include "../../Platform/OpenGL/Render/Buffer/UniformBuffer_GL.h"

namespace FakeReal {

	UniformBuffer* UniformBuffer::Create(unsigned int uiSize, unsigned int uiBinding)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::OpenGL: return new UniformBuffer_GL(uiSize, uiBinding);
		}
		assert(0);
		return nullptr;
	}

}
