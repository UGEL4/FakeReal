#include "VertexBuffer.h"
#include "../Renderer.h"
#include "../RenderResource/RenderResource.h"
#include "../../Graphic/Stream/Property.h"
#include <iostream>

namespace FakeReal {
	IMPLEMENT_RTTI(VertexBuffer, Object);
	IMPLEMENT_INITIAL_BEGIN(VertexBuffer)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(VertexBuffer, Object)
	REGISTER_PROPERTY(mBufferLayout, "BufferLayout", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pData[0], "Position Data", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pData[1], "Normal Data", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pData[2], "TexCoord Data", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pData[3], "Blend Weight Data", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pData[4], "Blend Index Data", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_uiVertexNum, "Vertices Count", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;

	VertexBuffer::VertexBuffer()
	{
		m_uiVertexNum = 0;
		//m_uiOneVertexSize = 0;
		for (int i = 0; i < VertexFormat::VF_MAX; i++)
		{
			m_pData[i].clear();
		}
		m_pID = nullptr;
		std::cout << "¹¹ÔìVertexBuffer" << this << std::endl;
	}

	VertexBuffer::~VertexBuffer()
	{
		for (int i = 0; i < VertexFormat::VF_MAX; i++)
		{
			for (auto& p : m_pData[i])
			{
				if (p)
					delete p;
			}
			m_pData[i].clear();
		}
		if (m_pID)
			delete m_pID;
		std::cout << "Îö¹¹VertexBuffer" << this << std::endl;
	}

	void VertexBuffer::Bind() const
	{
		if (m_pID)
		{
			VertexBufferID* pID = (VertexBufferID*)m_pID;
			pID->Bind();
			//std::cout << "BindBuffer " << this << ", " << pID << std::endl;
		}
	}

	void VertexBuffer::Unind() const
	{
		if (m_pID)
		{
			VertexBufferID* pID = (VertexBufferID*)m_pID;
			pID->Unbind();
		}
	}

	unsigned int VertexBuffer::GetBufferDataSize() const
	{
		return m_uiVertexNum * mBufferLayout.GetStride();
	}

	unsigned char* VertexBuffer::GetBufferData() const
	{
		unsigned int uiSize = GetBufferDataSize();
		if (uiSize == 0)
			return nullptr;
		const std::vector<LayoutElement>& elements = mBufferLayout.GetElements();
		unsigned char* data = new unsigned char[uiSize];
		if (!data)
			return nullptr;
		unsigned char* pos = data;
		for (unsigned int i = 0; i < m_uiVertexNum; i++)
		{
			for (const LayoutElement& e : elements)
			{
				if (m_pData[e.m_Semantics][e.m_SemanticsIndex] || m_pData[e.m_Semantics][e.m_SemanticsIndex]->GetData())
				{
					unsigned int size = ShaderDataTypeSize(e.m_Type);
					memcpy_s(pos, size, m_pData[e.m_Semantics][e.m_SemanticsIndex]->GetData() + size * i, size);
					pos += size;
				}
			}
		}
		return data;
	}

	unsigned int VertexBuffer::GetSemanticsNum(VertexFormat Semantics) const
	{
		unsigned int uiNum = 0;
		const std::vector<LayoutElement>& layouts = mBufferLayout.GetElements();
		for (unsigned int i = 0; i < layouts.size(); i++)
		{
			if (layouts[i].m_Semantics == Semantics)
			{
				uiNum++;
			}
		}
		return uiNum;
	}

	bool VertexBuffer::HavePositionData(size_t level /*= 0*/) const
	{
		return GetSemanticsNum(VertexFormat::VF_POSITION) > level;
	}

	bool VertexBuffer::HaveNormalData(size_t level /*= 0*/) const
	{
		return GetSemanticsNum(VertexFormat::VF_NORMAL) > level;
	}

	bool VertexBuffer::HaveTexCoordData(unsigned int uiLevel /*= 0*/) const
	{
		return GetSemanticsNum(VertexFormat::VF_TEXCOORD) > uiLevel;
	}

	bool VertexBuffer::HaveBlendWeightData() const
	{
		return GetSemanticsNum(VertexFormat::VF_BLENDWEIGHT) > 0;
	}

	bool VertexBuffer::HaveBlendIndicesData() const
	{
		return GetSemanticsNum(VertexFormat::VF_BLENDINDICES) > 0;
	}

	DataBuffer* VertexBuffer::GetPositionData(size_t level /*= 0*/) const
	{
		if (HavePositionData(level))
			return m_pData[VertexFormat::VF_POSITION][level];
		else
			return nullptr;
	}

	DataBuffer* VertexBuffer::GetNormalData(size_t level /*= 0*/) const
	{
		if (HaveNormalData(level))
			return m_pData[VertexFormat::VF_NORMAL][level];
		else
			return nullptr;
	}

	DataBuffer* VertexBuffer::GetTexCoordData(size_t level /*= 0*/) const
	{
		if (HaveTexCoordData(level))
			return m_pData[VertexFormat::VF_TEXCOORD][level];
		else
			return nullptr;
	}

	DataBuffer* VertexBuffer::GetBlendWeightData() const
	{
		if (HaveBlendWeightData())
			return m_pData[VertexFormat::VF_BLENDWEIGHT][0];
		else
			return nullptr;
	}

	DataBuffer* VertexBuffer::GetBlendIndicesData() const
	{
		if (HaveBlendIndicesData())
			return m_pData[VertexFormat::VF_BLENDINDICES][0];
		else
			return nullptr;
	}

	bool VertexBuffer::SetData(VertexFormat Semantics, DataBuffer* pData)
	{
		if (!pData || Semantics >= VertexFormat::VF_MAX || Semantics < VertexFormat::VF_POSITION)
			return false;
		if (!pData->GetData())
			return false;

		if (Semantics == VertexFormat::VF_POSITION)
		{
			m_pData[Semantics].emplace_back(pData);
		}
		else if (Semantics == VertexFormat::VF_NORMAL)
		{
			m_pData[Semantics].emplace_back(pData);
		}
		else if (Semantics == VertexFormat::VF_TEXCOORD)
		{
			m_pData[Semantics].emplace_back(pData);
		}
		else if (Semantics == VertexFormat::VF_BLENDWEIGHT)
		{
			if (!m_pData[Semantics].size())
				m_pData[Semantics].emplace_back(pData);
			else
				return false;
		}
		else if (Semantics == VertexFormat::VF_BLENDINDICES)
		{
			if (!m_pData[Semantics].size())
				m_pData[Semantics].emplace_back(pData);
			else
				return false;
		}

		if (m_uiVertexNum == 0)
		{
			m_uiVertexNum = pData->GetDataNum();
		}
		else
		{
			if (m_uiVertexNum != pData->GetDataNum())
				return false;
		}

		//m_uiOneVertexSize += ShaderDataTypeSize(pData->GetDataType());

		return true;
	}

}