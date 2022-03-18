#pragma once
#include <stdint.h>
#include <assert.h>
#include <initializer_list>
#include <vector>
#include "../../Core/CoreMarco.h"
#include "../../Graphic/Object.h"
namespace FakeReal {
	enum VertexFormat
	{
		VF_POSITION,
		VF_NORMAL,
		VF_TEXCOORD,
		VF_BLENDWEIGHT,
		VF_BLENDINDICES,
		VF_MAX
	};

	enum class ShaderDataType
	{
		None, Byte, UByte, Bool, Int, Int2, Int3, Int4, UInt, UInt2, UInt3, UInt4, Float, Float2, Float3, Float4, Mat3f, Mat4f, Max
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Byte:		return 1;
		case ShaderDataType::UByte:		return 1;
		case ShaderDataType::Bool:		return 1;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 8;
		case ShaderDataType::Int3:		return 12;
		case ShaderDataType::Int4:		return 16;
		case ShaderDataType::UInt:		return 4;
		case ShaderDataType::UInt2:		return 8;
		case ShaderDataType::UInt3:		return 12;
		case ShaderDataType::UInt4:		return 16;
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 8;
		case ShaderDataType::Float3:	return 12;
		case ShaderDataType::Float4:	return 16;
		case ShaderDataType::Mat3f:		return 36;
		case ShaderDataType::Mat4f:		return 64;
		}
		assert(0);
		return 0;
	}

	struct FR_ENGINE_API LayoutElement : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL

		LayoutElement()
		{
			m_Type = ShaderDataType::None;
			m_uiSize = 0;
			m_uiOffset = 0;
			m_Semantics = VF_MAX;
			m_SemanticsIndex = 0;
			m_bNormalize = false;
		}
		LayoutElement(ShaderDataType type, VertexFormat Semantics, unsigned int SemanticsIndex = 0, bool normalize = false)
			:m_Type(type)
			,m_uiSize(ShaderDataTypeSize(type))
			,m_uiOffset(0)
			,m_Semantics(Semantics)
			,m_SemanticsIndex(SemanticsIndex)
			,m_bNormalize(normalize)
		{

		}

		static unsigned int GetDataSize()
		{
			return sizeof(ShaderDataType) + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(bool);
		}

		uint32_t ShaderDataTypeComponent() const
		{
			switch (m_Type)
			{
			case ShaderDataType::Byte:		return 1;
			case ShaderDataType::UByte:		return 1;
			case ShaderDataType::Bool:		return 1;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::UInt:		return 1;
			case ShaderDataType::UInt2:		return 2;
			case ShaderDataType::UInt3:		return 3;
			case ShaderDataType::UInt4:		return 4;
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3f:		return 3;
			case ShaderDataType::Mat4f:		return 4;
			}
			assert(0);
			return 0;
		}

		ShaderDataType m_Type;
		unsigned int m_uiSize;
		unsigned int m_uiOffset;
		VertexFormat m_Semantics;
		unsigned int m_SemanticsIndex;
		bool m_bNormalize;
	};
	FR_TYPE_MARCO(LayoutElement)

	class FR_ENGINE_API BufferLayout : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		BufferLayout() 
		{
			m_uiStride = 0;
			m_Elements.clear();
		}
		BufferLayout(std::initializer_list<LayoutElement> elements)
			:m_Elements(elements)
		{
			CalculateStrideAndOffset();
		}

		void AddElement(const LayoutElement& e);

		const std::vector<LayoutElement>& GetElements() const { return m_Elements; }
		std::vector<LayoutElement>& GetElements() { return m_Elements; }
		uint32_t GetStride() const { return m_uiStride; }
	private:
		void CalculateStrideAndOffset()
		{
			uint32_t offset = 0;
			m_uiStride = 0;

			for (auto& e : m_Elements)
			{
				e.m_uiOffset	= offset;
				offset			+= e.m_uiSize;
				m_uiStride		+= e.m_uiSize;
			}
		}
	private:
		std::vector<LayoutElement> m_Elements;
		uint32_t m_uiStride;
	};
	FR_TYPE_MARCO(BufferLayout)

}