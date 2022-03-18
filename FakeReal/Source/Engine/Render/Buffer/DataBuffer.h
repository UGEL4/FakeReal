#pragma once

#include "../../Graphic/Object.h"
#include "BufferLayout.h"
#include "../../Core/CoreMarco.h"

namespace FakeReal {
	class FR_ENGINE_API DataBuffer : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		DataBuffer();
		~DataBuffer();

	public:
		bool SetData(const void* pData, unsigned int uiNum, ShaderDataType SDT);
		unsigned char* GetData() const { return m_pData; }
		unsigned int GetDataSize() const { return m_uiSize; }
		unsigned int GetDataNum() const { return m_uiNum; }
		ShaderDataType GetDataType() const { return m_SDT; }

	private:
		ShaderDataType m_SDT;
		unsigned int m_uiNum;
		unsigned int m_uiSize;
		unsigned char *m_pData;
	};
	FR_TYPE_MARCO(DataBuffer)
}