#pragma once

#include "../../../../Render/Buffer/VertexArray.h"
#include "../../../../Core/CoreMarco.h"
#include <stdint.h>
namespace FakeReal {
	
	class FR_ENGINE_API VertexArray_GL : public VertexArray
	{
	public:
		VertexArray_GL();
		~VertexArray_GL();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void AddBufferLayout(const VertexBuffer* pVertexBuffer, const BufferLayout* pBufferLayout) override;

	private:
		uint32_t m_RandererID;
	};
}