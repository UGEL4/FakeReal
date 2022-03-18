#include "RenderAPI.h"
#include "../Platform/OpenGL/Render/RenderAPI_GL.h"
namespace FakeReal {

	RenderAPI::API RenderAPI::ms_API = RenderAPI::API::OpenGL;
	RenderAPI* RenderAPI::Create()
	{
		switch (ms_API)
		{
		case API::None: return nullptr;
		case API::OpenGL: return new RenderAPI_GL();
		case API::DirectX12: return nullptr;
		}
		return nullptr;
	}


}