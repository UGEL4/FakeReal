#pragma once
#include <stdint.h>
#include "../../Core/CoreMarco.h"
#include "../../Graphic/Object.h"
#include "DataBuffer.h"

namespace FakeReal {
	class ResourceIdentifier;
	class FR_ENGINE_API IndexBuffer : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		IndexBuffer();
		IndexBuffer(DataBuffer* pData);
		~IndexBuffer();

		virtual void Bind() const;
		virtual void Unind() const;
		void SetIndexData(DataBuffer* pData);
		DataBuffer* GetIndexData() const { return m_pIndexData; }
		unsigned int GetIndexCount() const;
		void SetResourceIdentifier(ResourceIdentifier* pID) { m_pID = pID; }
	private:
		DataBuffer* m_pIndexData;//Ë÷ÒýÊý¾Ý
		ResourceIdentifier* m_pID;
	};
	FR_TYPE_MARCO(IndexBuffer)
}