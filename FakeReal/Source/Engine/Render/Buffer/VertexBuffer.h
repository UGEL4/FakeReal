#pragma once

#include "BufferLayout.h"
#include "DataBuffer.h"
#include "../../Core/CoreMarco.h"

namespace FakeReal {
	class ResourceIdentifier;
	class FR_ENGINE_API VertexBuffer : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		VertexBuffer();
		~VertexBuffer();

		void Bind() const;
		void Unind() const;
		//只在第一次初始化顶点数据时调用，不建议其他时候修改，否则可能会破坏顶点数据分布格式
		void SetLayout(const BufferLayout& BufferLayout) { mBufferLayout = BufferLayout; }
		const BufferLayout& GetBufferLayout() const { return mBufferLayout; }

		unsigned int GetBufferDataSize() const;
		//获取顶点数据{pos, normal, texcoord, weight, index}，需要使用者释放数据内存
		unsigned char* GetBufferData() const;

		unsigned int GetSemanticsNum(VertexFormat Semantics)const;
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
		void SetResourceIdentifier(ResourceIdentifier* pID) { m_pID = pID; }

	private:
		BufferLayout mBufferLayout;
		std::vector<DataBuffer*> m_pData[VertexFormat::VF_MAX];//数据
		unsigned int m_uiVertexNum;
		//unsigned int m_uiOneVertexSize;
		ResourceIdentifier* m_pID;//渲染资源
	};
	FR_TYPE_MARCO(VertexBuffer)
}