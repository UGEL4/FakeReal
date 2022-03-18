#include "DataBuffer.h"
#include "../../Graphic/Stream/Property.h"

namespace FakeReal {
	IMPLEMENT_RTTI(DataBuffer, Object);
	IMPLEMENT_INITIAL_BEGIN(DataBuffer)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(DataBuffer, Object)
	REGISTER_PROPERTY(m_SDT, "DataType", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_uiNum, "DataNum", Property::F_SAVE_LOAD_CLONE)
	REGISTER_DATA_PROPERTY(m_pData, m_uiSize, "Data")
	END_ADD_PROPERTY

	DataBuffer::DataBuffer()
	{
		m_SDT = ShaderDataType::Float;
		m_uiNum = 0;
		m_uiSize = 0;
		m_pData = nullptr;
		std::cout << "¹¹ÔìDataBuffer:" << this << std::endl;
	}

	DataBuffer::~DataBuffer()
	{
		if (m_pData)
			delete[] m_pData;
		std::cout << "Îö¹¹DataBuffer:" << this << std::endl;
	}

	bool DataBuffer::SetData(const void* pData, unsigned int uiNum, ShaderDataType SDT)
	{
		if (SDT >= ShaderDataType::Max || !pData || !uiNum)
			return false;

		m_SDT = SDT;
		m_uiNum = uiNum;

		if (m_pData)
			delete m_pData;
		unsigned int size = ShaderDataTypeSize(m_SDT) * uiNum;
		m_pData = new unsigned char[size];

		if (!m_pData)
			return false;
		memcpy_s(m_pData, size, pData, size);
		m_uiSize = size;
		return true;
	}

}