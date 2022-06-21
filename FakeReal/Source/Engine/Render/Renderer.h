#pragma once

#include <glm/glm.hpp>
#include "RenderAPI.h"
#include "Buffer/UniformBuffer.h"
#include "../Core/CoreMarco.h"
#include <glad/glad.h>
namespace FakeReal {

	struct CameraData
	{
		glm::mat4 viewProjection;

	};

	class VertexArray;
	class Shader;
	class Texture;
	class SamplerState;
	class TextureID;
	class VertexBufferID;
	class VertexBuffer;
	class VertexArrayID;
	class IndexBuffer;
	class IndexBufferID;
	class FR_ENGINE_API Renderer
	{
	public:

		enum DepthFunc
		{
			DF_LESS,
			DF_LESS_EQUAL,
			DF_MAX
		};

		enum StencilTestFunc
		{
			STF_NEVER,
			STF_LESS,
			STF_LESS_EQUAL,
			STF_GREATER,
			STF_GREATER_EQUAL,
			STF_EQUAL,
			STF_NOT_EQUAL,
			STF_ALWAYS,
			STF_MAX
		};

		enum StencilTestOperatorFunc
		{
			STOF_ZERO,
			STOF_KEEP,
			STOF_REPLACE,
			STOF_INCR,
			STOF_INCR_WRAP,
			STOF_DECR,
			STOF_DECR_WRAP,
			STOF_INVERT,
			STOF_MAX
		};

		static void Init();
		static void Release();
		static void SetClearColor(float r, float g, float b, float a);
		static void Clear();
		static void SetViewPort(int x, int y, int width, int height);
		static void Submit(const Shader& shader, const VertexArray& va, unsigned int count);
		static void OnWindowResize(unsigned int uiWidth, unsigned int uiHeight);

		static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

		static void BeginScene(glm::mat4& viewProj);
		static void EndScene();
		static bool IsRendererInited();

		static void SetDepthFunc(unsigned int DepthFuncEnum);
		static void EnableDepthTest(bool enable);
		static void DepthMask(bool mask);

		static void EnableStencilTest(bool enable);
		static void StencilMask(unsigned int Mask);
		static void SetStencilTestFunc(StencilTestFunc FuncEnum, int RefValue, unsigned int Mask);
		static void SetStencilTestOperator(StencilTestOperatorFunc StencilFailFuncEnum, StencilTestOperatorFunc DepthFailFuncEnum, StencilTestOperatorFunc AllPassFuncEnum);

		static TextureID* CreateTextureID(const Texture* pTexture, const SamplerState* pSamplerState);
		static VertexBufferID* CreateVertexBufferID(const VertexBuffer* pVB, bool BatchVertex = false);
		static VertexArrayID* CreateVertexArrayID();
		static IndexBufferID* CreateIndexBufferID(const IndexBuffer* pIB);
		
		static uint16_t GetTextureFormat(unsigned int originalFormat);

	private:
		Renderer() {}
		static uint16_t GL_GetTextureFormat(GLenum format);

	public:
		static CameraData s_CameraData;
		static UniformBuffer* s_pUniformBuffer;
	};
}
