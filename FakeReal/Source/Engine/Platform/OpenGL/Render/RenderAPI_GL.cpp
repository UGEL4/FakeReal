#include "RenderAPI_GL.h"
#include <glad/glad.h>
#include "../../../Render/Buffer/VertexArray.h"
namespace FakeReal {

	unsigned int RenderAPI_GL::ms_DepthFuncEnum[Renderer::DF_MAX] =
	{
		GL_LESS,
		GL_LEQUAL
	};

	unsigned int RenderAPI_GL::ms_StencilFuncEnum[Renderer::STF_MAX] =
	{
		GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_GEQUAL, GL_NOTEQUAL, GL_ALWAYS
	};

	unsigned int RenderAPI_GL::ms_StencilTestOperatorEnum[Renderer::STOF_MAX] =
	{
		GL_ZERO, GL_KEEP, GL_REPLACE, GL_INCR, GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT
	};

	void RenderAPI_GL::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void RenderAPI_GL::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void RenderAPI_GL::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void RenderAPI_GL::SetViewPort(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void RenderAPI_GL::DrawIndex(const VertexArray* pVa, unsigned count)
	{
		//pVa->Bind();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void RenderAPI_GL::SetDepthFunc(unsigned int DepthFuncEnum)
	{
		if (DepthFuncEnum < Renderer::DF_MAX)
			glDepthFunc(ms_DepthFuncEnum[DepthFuncEnum]);
	}

	void RenderAPI_GL::EnableDepthTest(bool enable)
	{
		if (enable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void RenderAPI_GL::EnableStentilTest(bool enable)
	{
		if (enable)
		{
			glEnable(GL_STENCIL_TEST);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}

	void RenderAPI_GL::DepthMask(bool mask)
	{
		if (mask)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}
	}

	void RenderAPI_GL::StencilMask(unsigned int Mask)
	{
		glStencilMask(Mask);
	}

	void RenderAPI_GL::SetStencilTestFunc(unsigned int FuncEnum, int RefValue, unsigned int Mask)
	{
		glStencilFunc(ms_StencilFuncEnum[FuncEnum], RefValue, Mask);
	}

	void RenderAPI_GL::SetStencilTestOperator(unsigned int StencilFailFuncEnum, unsigned int DepthFailFuncEnum, unsigned int AllPassFuncEnum)
	{
		glStencilOp(ms_StencilTestOperatorEnum[StencilFailFuncEnum], ms_StencilTestOperatorEnum[DepthFailFuncEnum], ms_StencilTestOperatorEnum[AllPassFuncEnum]);
	}

}