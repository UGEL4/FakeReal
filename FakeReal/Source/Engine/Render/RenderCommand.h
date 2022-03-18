#pragma once

namespace FakeReal {
	class RenderAPI;
	class VertexArray;
	class RenderCommand
	{
	public:
		static void Init();
		static void Release();
		static void SetClearColor(float r, float g, float b, float a);
		static void Clear();
		static void SetViewPort(int x, int y, int width, int height);
		static void DrawIndex(const VertexArray* pVa, unsigned int count);
		static bool IsInited();
		static void SetDepthFunc(unsigned int DepthFuncEnum);
		static void EnableDepthTest(bool enable);
		static void EnableStencilTest(bool enable);
		static void DepthMask(bool mask);
		static void StencilMask(unsigned int Mask);
		static void SetStencilTestFunc(unsigned int FuncEnum, int RefValue, unsigned int Mask);
		static void SetStencilTestOperator(unsigned int StencilFailFuncEnum, unsigned int DepthFailFuncEnum, unsigned int AllPassFuncEnum);

	private:
		RenderCommand() {}
		static RenderAPI* ms_pRenderAPI;
	};
}
