#pragma once

#include "../../../../Render/Buffer/UniformBuffer.h"
#include "../../../../Core/CoreMarco.h"

namespace FakeReal {
	class FR_ENGINE_API UniformBuffer_GL : public UniformBuffer
	{
	public:
		UniformBuffer_GL();
		UniformBuffer_GL(unsigned int uiSize, unsigned int uiBinding);
		~UniformBuffer_GL();

		virtual void SetData(const void* pData, unsigned int uiSize, unsigned int uiOffset = 0) override;
		virtual void Bind() const override;
		virtual void Unbind()const override;
		virtual void Initial(unsigned int uiSize, unsigned int uiBinding) override;
	private:
		unsigned int m_RendererID;
	};
}
