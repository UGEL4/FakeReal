#pragma once
//#include "VertexArray.h"
namespace FakeReal {
	class VertexArray;
	class RenderAPI
	{
	public:
		enum class API
		{
			None, OpenGL, DirectX12
		};

		virtual ~RenderAPI() = default;

		virtual void Init() = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;
		virtual void SetViewPort(int x, int y, int width, int height) = 0;
		virtual void DrawIndex(const VertexArray* pVa, unsigned int count) = 0;
		virtual void SetDepthFunc(unsigned int DepthFuncEnum) = 0;
		virtual void EnableDepthTest(bool enable) = 0;
		virtual void EnableStentilTest(bool enable) = 0;
		virtual void DepthMask(bool mask) = 0;
		virtual void StencilMask(unsigned int Mask) = 0;
		virtual void SetStencilTestFunc(unsigned int FuncEnum, int RefValue, unsigned int Mask) = 0;
		virtual void SetStencilTestOperator(unsigned int StencilFailFuncEnum, unsigned int DepthFailFuncEnum, unsigned int AllPassFuncEnum) = 0;

	public:
		static RenderAPI* Create();
		static API GetAPI() { return ms_API; }

	private:
		static API ms_API;
	};
}