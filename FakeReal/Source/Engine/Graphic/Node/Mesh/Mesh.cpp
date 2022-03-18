#include "Mesh.h"
#include <iostream>
#include "../../Stream/Property.h"
#include "../../../Render/Renderer.h"
#include "../../../Render/RenderResource/RenderResource.h"
namespace FakeReal {
	IMPLEMENT_RTTI(Mesh, Object);
	IMPLEMENT_INITIAL_BEGIN(Mesh)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(Mesh, Object)
	REGISTER_PROPERTY(m_pVertexBuffer, "VertexBuffer", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pIndexBuffer, "IndexBuffer", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_Textures, "TexturesResource", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	Mesh::Mesh()
		: m_pVertexArray(nullptr)
		, m_pVertexBuffer(nullptr)
		, m_pIndexBuffer(nullptr)
	{
		m_Textures.clear();
		//m_uiVertexNum = 0;
		std::cout << "构造Mesh" << std::endl;
	}

	Mesh::~Mesh()
	{
		if (m_pVertexArray)
			delete m_pVertexArray;
		if (m_pVertexBuffer)
			delete m_pVertexBuffer;
		if (m_pIndexBuffer)
			delete m_pIndexBuffer;
		for (auto& texture : m_Textures)
		{
			if (texture) delete texture;
		}
		
		std::cout << "销毁Mesh" << std::endl;
	}

	bool Mesh::PostLoad(void* pData /*= nullptr*/)
	{
		/*BufferLayout layout;
		unsigned int offSet = 0;
		for (unsigned int i = 0; i < VertexFormat::VF_MAX; i++)
		{
			if (m_pData[i].size() > 0)
			{
				LayoutElement LE;
				LE.m_Type = m_pData[i][0]->GetDataType();
				LE.m_uiSize = ShaderDataTypeSize(LE.m_Type);
				LE.m_uiOffset = offSet;
				LE.m_Semantics = (VertexFormat)i;
				LE.m_SemanticsIndex = 0;//TODO:支持多个数据通道
				LE.m_bNormalize = false;
				offSet += LE.m_uiSize;
				layout.AddElement(LE);
			}
		}
		unsigned int memSize = layout.GetStride() * m_uiVertexNum;
		unsigned char* Temp = new unsigned char[memSize];
		unsigned char* pos = Temp;
		for (unsigned int i = 0; i < m_uiVertexNum; i++)
		{
			for (unsigned int j = 0; j < VertexFormat::VF_MAX; j++)
			{
				if (m_pData[j].size() > 0)
				{
					unsigned int size = ShaderDataTypeSize(m_pData[j][0]->GetDataType());
					memcpy_s(pos, size, m_pData[j][0]->GetData() + size * i, size);
					pos += size;
				}
			}
		}

		m_pVertexArray = VertexArray::Create();
		m_pVertexBuffer = VertexBuffer::Create(Temp, memSize);
		m_pVertexBuffer->SetLayout(layout);
		m_pVertexArray->AddBufferLayout(m_pVertexBuffer, &layout);

		m_pIndexBuffer = IndexBuffer::Create(m_pIndexData->GetData(), m_pIndexData->GetDataNum());

		//Texture2D* pTex = Texture2D::Create("Resources/Stormtrooper_D.png");
		//m_Textures2D.emplace_back(pTex);

		delete[] Temp;
		*/

		ResourceIdentifier* pVAID = Renderer::CreateVertexArrayID();
		m_pVertexArray = new VertexArray;
		m_pVertexArray->SetResourceIdentifier(pVAID);

		ResourceIdentifier* pVBID = Renderer::CreateVertexBufferID(m_pVertexBuffer, true);
		m_pVertexBuffer->SetResourceIdentifier(pVBID);

		ResourceIdentifier* pIBID = Renderer::CreateIndexBufferID(m_pIndexBuffer);
		m_pIndexBuffer->SetResourceIdentifier(pIBID);
		return true;
	}

	bool Mesh::PostClone(Object * pObjectSrc)
	{
		/*unsigned int memSize = pSrcMesh->m_pVertexBuffer->GetBufferLayout().GetStride() * m_uiVertexNum;
		unsigned char* pData = new unsigned char[memSize];
		unsigned char* Temp = pData;
		for (unsigned int i = 0; i < m_uiVertexNum; i++)
		{
			for (unsigned int j = 0; j < VertexFormat::VF_MAX; j++)
			{
				if (m_pData[j].size() > 0)
				{
					unsigned int size = ShaderDataTypeSize(m_pData[j][0]->GetDataType());
					memcpy_s(Temp, size, m_pData[j][0]->GetData() + size * i, size);
					Temp += size;
				}
			}
		}*/

		if (Renderer::IsRendererInited())
		{
			ResourceIdentifier* pVAID = Renderer::CreateVertexArrayID();
			m_pVertexArray = new VertexArray;
			m_pVertexArray->SetResourceIdentifier(pVAID);

			ResourceIdentifier* pVBID = Renderer::CreateVertexBufferID(m_pVertexBuffer, true);
			m_pVertexBuffer->SetResourceIdentifier(pVBID);

			ResourceIdentifier* pIBID = Renderer::CreateIndexBufferID(m_pIndexBuffer);
			m_pIndexBuffer->SetResourceIdentifier(pIBID);
		}
		return true;
	}

	void Mesh::SetVertexBuffer(VertexBuffer* pBuffer)
	{
		m_pVertexBuffer = pBuffer;
		//m_pVertexBuffer->Bind();
	}

	void Mesh::SetIndexBuffer(IndexBuffer* pBuffer)
	{
		m_pIndexBuffer = pBuffer;
		//m_pIndexBuffer->Bind();
	}

	unsigned int Mesh::GetSemanticsNum(VertexFormat Semantics) const
	{
		if (!m_pVertexBuffer)
		{
			return 0;
		}
		else
		{
			return m_pVertexBuffer->GetSemanticsNum(Semantics);
		}
	}

	bool Mesh::HavePositionData(size_t level) const
	{
		return GetSemanticsNum(VertexFormat::VF_POSITION) > level;
	}

	bool Mesh::HaveNormalData(size_t level) const
	{
		return GetSemanticsNum(VertexFormat::VF_NORMAL) > level;
	}

	bool Mesh::HaveTexCoordData(unsigned int uiLevel) const
	{
		return GetSemanticsNum(VertexFormat::VF_TEXCOORD) > uiLevel;
	}

	bool Mesh::HaveBlendWeightData() const
	{
		return GetSemanticsNum(VertexFormat::VF_BLENDWEIGHT) > 0;
	}

	bool Mesh::HaveBlendIndicesData() const
	{
		return GetSemanticsNum(VertexFormat::VF_BLENDINDICES) > 0;
	}

	DataBuffer* Mesh::GetPositionData(size_t level) const
	{
		if (m_pVertexBuffer)
			return m_pVertexBuffer->GetPositionData(level);
		else
			return nullptr;
	}

	DataBuffer* Mesh::GetNormalData(size_t level) const
	{
		if (m_pVertexBuffer)
			return m_pVertexBuffer->GetNormalData(level);
		else
			return nullptr;
	}

	DataBuffer* Mesh::GetTexCoordData(size_t level) const
	{
		if (m_pVertexBuffer)
			return m_pVertexBuffer->GetTexCoordData(level);
		else
			return nullptr;
	}

	DataBuffer* Mesh::GetBlendWeightData() const
	{
		if (m_pVertexBuffer)
			return m_pVertexBuffer->GetBlendWeightData();
		else
			return nullptr;
	}

	DataBuffer* Mesh::GetBlendIndicesData() const
	{
		if (m_pVertexBuffer)
			return m_pVertexBuffer->GetBlendIndicesData();
		else
			return nullptr;
	}

	bool Mesh::SetData(VertexFormat Semantics, DataBuffer* pData)
	{
		if (m_pVertexBuffer)
		{
			return m_pVertexBuffer->SetData(Semantics, pData);
		}

		return false;
	}

	void Mesh::SetIndexData(DataBuffer* pData)
	{
		if (!pData || !pData->GetData() || !m_pIndexBuffer)
			return;
		m_pIndexBuffer->SetIndexData(pData);
	}

	unsigned int Mesh::GetIndexCount() const
	{
		if (m_pIndexBuffer)
			return m_pIndexBuffer->GetIndexCount();
		else
			return 0;
	}

}
