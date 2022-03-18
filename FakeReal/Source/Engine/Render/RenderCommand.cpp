#include "RenderCommand.h"
//#include "VertexArray.h"
#include "RenderAPI.h"
#include "Buffer/VertexArray.h"
#include "../Platform/OpenGL/Render/RenderAPI_GL.h"

namespace FakeReal {

	RenderAPI* RenderCommand::ms_pRenderAPI = nullptr;

	void RenderCommand::Init()
	{
		ms_pRenderAPI = RenderAPI::Create();
		ms_pRenderAPI->Init();
	}

	void RenderCommand::Release()
	{
		delete ms_pRenderAPI;
	}

	void RenderCommand::SetClearColor(float r, float g, float b, float a)
	{
		ms_pRenderAPI->SetClearColor(r, g, b, a);
	}

	void RenderCommand::Clear()
	{
		ms_pRenderAPI->Clear();
	}

	void RenderCommand::SetViewPort(int x, int y, int width, int height)
	{
		ms_pRenderAPI->SetViewPort(x, y, width, height);
	}

	void RenderCommand::DrawIndex(const VertexArray* pVa, unsigned int count)
	{
		ms_pRenderAPI->DrawIndex(pVa, count);
	}

	bool RenderCommand::IsInited()
	{
		return ms_pRenderAPI != nullptr;
	}

	void RenderCommand::SetDepthFunc(unsigned int DepthFuncEnum)
	{
		ms_pRenderAPI->SetDepthFunc(DepthFuncEnum);
	}

	void RenderCommand::EnableDepthTest(bool enable)
	{
		ms_pRenderAPI->EnableDepthTest(enable);
	}

	void RenderCommand::EnableStencilTest(bool enable)
	{
		ms_pRenderAPI->EnableStentilTest(enable);
	}

	void RenderCommand::DepthMask(bool mask)
	{
		ms_pRenderAPI->DepthMask(mask);
	}

	void RenderCommand::StencilMask(unsigned int Mask)
	{
		ms_pRenderAPI->StencilMask(Mask);
	}

	void RenderCommand::SetStencilTestFunc(unsigned int FuncEnum, int RefValue, unsigned int Mask)
	{
		ms_pRenderAPI->SetStencilTestFunc(FuncEnum, RefValue, Mask);
	}

	void RenderCommand::SetStencilTestOperator(unsigned int StencilFailFuncEnum, unsigned int DepthFailFuncEnum, unsigned int AllPassFuncEnum)
	{
		ms_pRenderAPI->SetStencilTestOperator(StencilFailFuncEnum, DepthFailFuncEnum, AllPassFuncEnum);
	}

}
