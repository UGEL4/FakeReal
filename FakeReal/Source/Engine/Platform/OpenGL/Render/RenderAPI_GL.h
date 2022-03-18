#pragma once

#include "../../../Render/RenderAPI.h"
#include "../../../Render/Renderer.h"
namespace FakeReal {
	class VertexArray;
	class RenderAPI_GL : public RenderAPI
	{
	public:
		virtual void Init() override;
		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void Clear() override;
		virtual void SetViewPort(int x, int y, int width, int height) override;
		virtual void DrawIndex(const VertexArray* pVa, unsigned count) override;
		virtual void SetDepthFunc(unsigned int DepthFuncEnum) override;
		virtual void EnableDepthTest(bool enable) override;
		virtual void EnableStentilTest(bool enable) override;
		virtual void DepthMask(bool mask) override;
		virtual void StencilMask(unsigned int Mask) override;
		virtual void SetStencilTestFunc(unsigned int FuncEnum, int RefValue, unsigned int Mask) override;
		virtual void SetStencilTestOperator(unsigned int StencilFailFuncEnum, unsigned int DepthFailFuncEnum, unsigned int AllPassFuncEnum) override;

	private:
		static unsigned int ms_DepthFuncEnum[Renderer::DF_MAX];
		static unsigned int ms_StencilFuncEnum[Renderer::STF_MAX];
		static unsigned int ms_StencilTestOperatorEnum[Renderer::STOF_MAX];
	};
}