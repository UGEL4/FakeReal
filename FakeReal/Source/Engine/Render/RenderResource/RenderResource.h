#pragma once
#include "ResourceBind.h"
#include "../../Core/CoreMarco.h"

namespace FakeReal {

	/************************************************************************/
	/*                       Texture                                        */
	/************************************************************************/
	class FR_ENGINE_API TextureID : public ResourceIdentifier
	{
	public:
		TextureID() {}
		virtual ~TextureID() {}

		virtual void* GetRenderResource() const = 0;
		virtual void Bind(unsigned int slot = 0) = 0;
		//virtual void SetRenderResource(void* data) = 0;
	};

	class Texture;
	class SamplerState;
	class FR_ENGINE_API TextureID_GL : public TextureID
	{
	public:
		enum
		{
			T2D,
			T3D,
			TCUBE,
			TMAX
		};
		TextureID_GL();
		~TextureID_GL();

		void CreateRenderResource(const Texture* pTexture, const SamplerState* pSamplerState);
		virtual void* GetRenderResource() const override;
		//virtual void SetRenderResource(void* data) override;
		virtual void Bind(unsigned int slot = 0) override;

	private:
		unsigned int mRendererID;
		unsigned int mTextureType;
	};

	/************************************************************************/
	/*                   VertexArray                                        */
	/************************************************************************/
	class VertexBuffer;
	class FR_ENGINE_API VertexArrayID : public ResourceIdentifier
	{
	public:
		VertexArrayID();
		~VertexArrayID();
		void CreateRenderResource();
		virtual void* GetRenderResource() const;
		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		unsigned int mRendererID;
	};

	/************************************************************************/
	/*							VertexBuffer							    */
	/************************************************************************/
	class VertexBuffer;
	class FR_ENGINE_API VertexBufferID : public ResourceIdentifier
	{
	public:
		VertexBufferID() {}
		virtual ~VertexBufferID() {}

		virtual void CreateRenderResource(const VertexBuffer* pVB, bool BatchVertex = false) = 0;
		virtual void* GetRenderResource() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class FR_ENGINE_API VertexBufferID_GL : public VertexBufferID
	{
	public:
		VertexBufferID_GL();
		~VertexBufferID_GL();

		virtual void CreateRenderResource(const VertexBuffer* pVB, bool BatchVertex = false) override;
		virtual void* GetRenderResource() const override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		//АэИз:111222333
		void VertexArrayAttributeBatch(const VertexBuffer* pVB);
		//АэИз:123123123
		void VertexArrayAttributeInterleave(const VertexBuffer* pVB);
		unsigned int mRendererID;
	};

	/************************************************************************/
	/*							IndexBuffer							        */
	/************************************************************************/
	class IndexBuffer;
	class FR_ENGINE_API IndexBufferID : public ResourceIdentifier
	{
	public:
		IndexBufferID() {}
		virtual ~IndexBufferID() {};

		virtual void CreateRenderResource(const IndexBuffer* pData) = 0;
		virtual void* GetRenderResource() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class FR_ENGINE_API IndexBufferID_GL : public IndexBufferID
	{
	public:
		IndexBufferID_GL();
		~IndexBufferID_GL();

		virtual void CreateRenderResource(const IndexBuffer* pData) override;
		virtual void* GetRenderResource() const override;
		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		unsigned int mRendererID;
	};
}