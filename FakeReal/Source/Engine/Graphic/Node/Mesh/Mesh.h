#pragma once

#include "../../Object.h"
#include "../../../Render/Buffer/VertexArray.h"
#include "../../../Render/Buffer/VertexBuffer.h"
#include "../../../Render/Buffer/IndexBuffer.h"
#include "../../../Render/Texture/TextureAllState.h"
#include "../../../Render/Buffer/BufferLayout.h"
#include "../../../Render/Buffer/DataBuffer.h"
#include <vector>

namespace FakeReal {
	class FR_ENGINE_API Mesh : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		Mesh();
		virtual ~Mesh();

		virtual bool PostLoad(void * pData = nullptr) override;
		virtual bool PostClone(Object * pObjectSrc) override;

		void SetVertexArray(VertexArray* pVertexArray) { m_pVertexArray = pVertexArray; }
		void SetVertexBuffer(VertexBuffer* pBuffer);
		void SetIndexBuffer(IndexBuffer* pBuffer);
		void SetTextures2D(const std::vector<TextureAllStateR*>& list) { m_Textures = list; }
		void AddTexture2D(TextureAllStateR* pTexStateR)
		{
			m_Textures.emplace_back(pTexStateR);
		}
		std::vector<TextureAllStateR*>& GetTextureList()
		{
			return m_Textures;
		}

		VertexArray* GetVertexArray() const { return m_pVertexArray; }
		VertexBuffer* GetVertexBuffer() const { return m_pVertexBuffer; }
		IndexBuffer* GetIndexBuffer() const { return m_pIndexBuffer; }

		unsigned int GetSemanticsNum(VertexFormat uiSemantics)const;
		bool HavePositionData(size_t level = 0) const;
		bool HaveNormalData(size_t level = 0) const;
		bool HaveTexCoordData(unsigned int uiLevel = 0) const;
		bool HaveBlendWeightData() const;
		bool HaveBlendIndicesData() const;
		DataBuffer* GetPositionData(size_t level = 0) const;
		DataBuffer* GetNormalData(size_t level = 0) const;
		DataBuffer* GetTexCoordData(size_t level = 0) const;
		DataBuffer* GetBlendWeightData() const;
		DataBuffer* GetBlendIndicesData() const;
		bool SetData(VertexFormat Semantics, DataBuffer* pData);
		void SetIndexData(DataBuffer* pData);
		//DataBuffer* GetIndexData() const { return m_pIndexData; }
		unsigned int GetIndexCount() const;
	protected:
		VertexArray* m_pVertexArray;
		VertexBuffer* m_pVertexBuffer;
		IndexBuffer* m_pIndexBuffer;
		std::vector<TextureAllStateR*> m_Textures;
		//unsigned int m_uiVertexNum;
	};
	FR_TYPE_MARCO(Mesh)
}
