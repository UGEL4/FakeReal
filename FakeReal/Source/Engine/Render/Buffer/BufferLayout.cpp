#include "BufferLayout.h"
#include "../../Graphic/Stream/Property.h"
namespace FakeReal
{
	IMPLEMENT_RTTI(LayoutElement, Object);
	IMPLEMENT_INITIAL_BEGIN(LayoutElement)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(LayoutElement, Object)
	REGISTER_ENUM_PROPERTY(m_Type, "ShaderDataType", "DataType", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_uiSize, "DataSize", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_uiOffset, "OffSet", Property::F_SAVE_LOAD_CLONE)
	REGISTER_ENUM_PROPERTY(m_Semantics, "VertexFormat", "Semantics", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_SemanticsIndex, "SemanticsIndex", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_bNormalize, "Normalize", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;


	IMPLEMENT_RTTI(BufferLayout, Object);
	IMPLEMENT_INITIAL_BEGIN(BufferLayout)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(BufferLayout, Object)
	REGISTER_PROPERTY(m_Elements, "Layout Elements", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_uiStride, "Stride", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;

	void BufferLayout::AddElement(const LayoutElement& e)
	{
		m_Elements.emplace_back(e);
		m_uiStride += e.m_uiSize;
	}
}
