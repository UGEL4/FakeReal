#include "VertexArray.h"
#include "VertexBuffer.h"
#include "../Renderer.h"
#include "../RenderResource/RenderResource.h"
#include "../../Graphic/Stream/Property.h"

namespace FakeReal {
	IMPLEMENT_RTTI(VertexArray, Object);
	IMPLEMENT_INITIAL_BEGIN(VertexArray)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(VertexArray, Object)
	END_ADD_PROPERTY;

	VertexArray::VertexArray()
	{
		m_pID = nullptr;
		std::cout << "¹¹ÔìVertexArray:" << this << std::endl;
	}

	VertexArray::~VertexArray()
	{
		if (m_pID)
			delete m_pID;
		std::cout << "Îö¹¹VertexArray:" << this << std::endl;
	}

	void VertexArray::Bind() const
	{
		if (m_pID)
		{
			((VertexArrayID*)m_pID)->Bind();
		}
	}

	void VertexArray::Unbind() const
	{
		if(m_pID)
		{
			((VertexArrayID*)m_pID)->Unbind();
		}
	}

}