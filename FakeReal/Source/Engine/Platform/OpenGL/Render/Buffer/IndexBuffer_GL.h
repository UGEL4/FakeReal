#pragma once

#include "../../../../Render/Buffer/IndexBuffer.h"
#include "../../../../Core/CoreMarco.h"

namespace FakeReal {
	class FR_ENGINE_API IndexBuffer_GL : public IndexBuffer
	{
	public:
		IndexBuffer_GL();
		IndexBuffer_GL(const void* data, uint32_t count);
		~IndexBuffer_GL();
		virtual void SetData(const void* data, uint32_t count) override;
		virtual void Bind() const override;
		virtual void Unind() const override;
		virtual unsigned int GetCount() const override { return m_uiCount; }
	private:
		uint32_t m_RendererID;
		uint32_t m_uiCount;
	};
}