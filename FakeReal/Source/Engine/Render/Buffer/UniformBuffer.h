#pragma once
#include "../../Core/CoreMarco.h"

namespace FakeReal {
	class FR_ENGINE_API UniformBuffer
	{
	public:
		UniformBuffer() {}
		virtual ~UniformBuffer() {};

		virtual void SetData(const void* pData, unsigned int uiSize, unsigned int uiOffset = 0) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Initial(unsigned int uiSize, unsigned int uiBinding) = 0;

		static UniformBuffer* Create(unsigned int uiSize, unsigned int uiBinding);
	};
}
