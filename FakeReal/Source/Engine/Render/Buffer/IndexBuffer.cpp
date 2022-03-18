#include "IndexBuffer.h"
#include <stdint.h>
#include "../Renderer.h"
#include "../RenderResource/RenderResource.h"
#include "../../Graphic/Stream/Property.h"

namespace FakeReal {
	IMPLEMENT_RTTI(IndexBuffer, Object);
	IMPLEMENT_INITIAL_BEGIN(IndexBuffer)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(IndexBuffer, Object)
	REGISTER_PROPERTY(m_pIndexData, "Indices Data", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;

	IndexBuffer::IndexBuffer()
	{
		m_pIndexData = nullptr;
		m_pID = nullptr;
		std::cout << "构造IndexBuffer:" << this << std::endl;
	}

	IndexBuffer::IndexBuffer(DataBuffer* pData)
	{
		m_pIndexData = pData;
		m_pID = nullptr;
		std::cout << "构造IndexBuffer:" << this << std::endl;
	}

	IndexBuffer::~IndexBuffer()
	{
		if (m_pIndexData)
			delete m_pIndexData;
		if (m_pID)
			delete m_pID;
		std::cout << "析构IndexBuffer:" << this << std::endl;
	}

	void IndexBuffer::Bind() const
	{
		if (m_pID)
		{
			((IndexBufferID*)m_pID)->Bind();
		}
	}

	void IndexBuffer::Unind() const
	{
		if (m_pID)
		{
			((IndexBufferID*)m_pID)->Unbind();
		}
	}

	void IndexBuffer::SetIndexData(DataBuffer* pData)
	{
		if (!pData || !pData->GetData())
			return;
		if (m_pIndexData && m_pIndexData->GetData())
			return;
		m_pIndexData = pData;
	}

	unsigned int IndexBuffer::GetIndexCount() const
	{
		if (m_pIndexData && m_pIndexData->GetData())
			return m_pIndexData->GetDataNum();
		else
			return 0;
	}

}