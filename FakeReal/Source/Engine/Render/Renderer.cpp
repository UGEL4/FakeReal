#include "Renderer.h"
#include "Shader/Shader.h"
#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/IndexBuffer.h"
#include "RenderCommand.h"
#include "RenderResource/RenderResource.h"

namespace FakeReal
{
	CameraData Renderer::s_CameraData;
	UniformBuffer* Renderer::s_pUniformBuffer = nullptr;

	void Renderer::Init()
	{
		RenderCommand::Init();
		s_pUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);
	}

	void Renderer::Release()
	{
		delete s_pUniformBuffer;
		RenderCommand::Release();
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
		RenderCommand::SetClearColor(r, g, b, a);
	}

	void Renderer::Clear()
	{
		RenderCommand::Clear();
	}

	void Renderer::SetViewPort(int x, int y, int width, int height)
	{
		RenderCommand::SetViewPort(x, y, width, height);
	}

	void Renderer::Submit(const Shader& shader, const VertexArray& va, unsigned int count)
	{
		shader.Bind();
		va.Bind();
		RenderCommand::DrawIndex(&va, count);
	}

	void Renderer::OnWindowResize(unsigned int uiWidth, unsigned int uiHeight)
	{
		RenderCommand::SetViewPort(0, 0, uiWidth, uiHeight);
	}

	void Renderer::BeginScene(glm::mat4& viewProj)
	{
		//s_CameraData.viewProjection = camera.GetViewProjectionMatrix();
		s_CameraData.viewProjection = viewProj;
		//s_pUniformBuffer->Bind();
		s_pUniformBuffer->SetData(&s_CameraData, sizeof(CameraData));
	}

	void Renderer::EndScene()
	{
		//s_pUniformBuffer->Unbind();
	}

	bool Renderer::IsRendererInited()
	{
		return RenderCommand::IsInited();
	}

	void Renderer::SetDepthFunc(unsigned int DepthFuncEnum)
	{
		RenderCommand::SetDepthFunc(DepthFuncEnum);
	}

	void Renderer::EnableDepthTest(bool enable)
	{
		RenderCommand::EnableDepthTest(enable);
	}

	void Renderer::EnableStencilTest(bool enable)
	{
		RenderCommand::EnableStencilTest(enable);
	}

	void Renderer::DepthMask(bool mask)
	{
		RenderCommand::DepthMask(mask);
	}

	void Renderer::StencilMask(unsigned int Mask)
	{
		RenderCommand::StencilMask(Mask);
	}

	void Renderer::SetStencilTestFunc(StencilTestFunc FuncEnum, int RefValue, unsigned int Mask)
	{
		RenderCommand::SetStencilTestFunc(FuncEnum, RefValue, Mask);
	}

	void Renderer::SetStencilTestOperator(StencilTestOperatorFunc StencilFailFuncEnum, StencilTestOperatorFunc DepthFailFuncEnum, StencilTestOperatorFunc AllPassFuncEnum)
	{
		RenderCommand::SetStencilTestOperator(StencilFailFuncEnum, DepthFailFuncEnum, AllPassFuncEnum);
	}

	TextureID* Renderer::CreateTextureID(const Texture* pTexture, const SamplerState* pSamplerState)
	{
		switch (GetAPI())
		{
		case RenderAPI::API::OpenGL:
		{
			TextureID_GL* pID = new TextureID_GL;
			pID->CreateRenderResource(pTexture, pSamplerState);
			return pID;
		}
		}
		return nullptr;
	}

	VertexBufferID* Renderer::CreateVertexBufferID(const VertexBuffer* pVB, bool BatchVertex)
	{
		switch (GetAPI())
		{
		case RenderAPI::API::OpenGL:
		{
			VertexBufferID_GL* pID = new VertexBufferID_GL;
			pID->CreateRenderResource(pVB, BatchVertex);
			return pID;
		}
		}
		return nullptr;
	}

	VertexArrayID* Renderer::CreateVertexArrayID()
	{
		switch (GetAPI())
		{
		case RenderAPI::API::OpenGL:
		{
			VertexArrayID* pID = new VertexArrayID;
			pID->CreateRenderResource();
			return pID;
		}
		}
		return nullptr;
	}

	IndexBufferID* Renderer::CreateIndexBufferID(const IndexBuffer* pIB)
	{
		switch (GetAPI())
		{
		case RenderAPI::API::OpenGL:
		{
			IndexBufferID* pID = new IndexBufferID_GL;
			pID->CreateRenderResource(pIB);
			return pID;
		}
		}
		return nullptr;
	}

}
