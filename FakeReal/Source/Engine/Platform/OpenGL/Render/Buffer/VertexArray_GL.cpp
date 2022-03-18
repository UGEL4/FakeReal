#include "VertexArray_GL.h"
#include <glad/glad.h>
#include <iostream>
namespace FakeReal {
	static GLenum ShaderDataTypeToGLenum(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Byte:		return GL_BYTE;
		case ShaderDataType::UByte:		return GL_UNSIGNED_BYTE;
		case ShaderDataType::Bool:		return GL_BOOL;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::UInt:
		case ShaderDataType::UInt2:
		case ShaderDataType::UInt3:
		case ShaderDataType::UInt4:		return GL_UNSIGNED_INT;
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3f:
		case ShaderDataType::Mat4f:		return GL_FLOAT;
		}
		assert(0);
		return 0;
	}
	
	VertexArray_GL::VertexArray_GL()
	{
		glGenVertexArrays(1, &m_RandererID);
		//glCreateVertexArrays(1, &m_RandererID); //gl 4.5
		std::cout << "¹¹ÔìOpenGLVertexArray" << std::endl;
	}

	VertexArray_GL::~VertexArray_GL()
	{
		glDeleteVertexArrays(1, &m_RandererID);
		std::cout << "Ïú»ÙOpenGLVertexArray" << std::endl;
	}

	void VertexArray_GL::Bind() const
	{
		glBindVertexArray(m_RandererID);
	}

	void VertexArray_GL::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray_GL::AddBufferLayout(const VertexBuffer* pVertexBuffer, const BufferLayout* pBufferLayout)
	{
		if (!pVertexBuffer || !pBufferLayout)
		{
			assert(0);
			return;
		}

		Bind();
		pVertexBuffer->Bind();

		uint32_t offser = 0;
		uint32_t index = 0;
		const auto& elements = pBufferLayout->GetElements();
		for (const auto& e : elements)
		{
			switch (e.m_Type)
			{
			case ShaderDataType::Byte:
			case ShaderDataType::UByte:
			case ShaderDataType::Bool:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::UInt:
			case ShaderDataType::UInt2:
			case ShaderDataType::UInt3:
			case ShaderDataType::UInt4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(
					index,
					e.ShaderDataTypeComponent(),
					ShaderDataTypeToGLenum(e.m_Type),
					pBufferLayout->GetStride(),
					(const void*)e.m_uiOffset
				);
				index++;
				break;
			}
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(
					index,
					e.ShaderDataTypeComponent(),
					ShaderDataTypeToGLenum(e.m_Type),
					e.m_bNormalize ? GL_TRUE : GL_FALSE,
					pBufferLayout->GetStride(),
					(const void*)e.m_uiOffset
				);
				index++;
				break;
			}
			case ShaderDataType::Mat3f:
			case ShaderDataType::Mat4f:
			{
				uint8_t count = e.ShaderDataTypeComponent();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(
						index,
						count,
						ShaderDataTypeToGLenum(e.m_Type),
						e.m_bNormalize ? GL_TRUE : GL_FALSE,
						pBufferLayout->GetStride(),
						(const void*)(e.m_uiOffset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				assert(0);
				break;
			}
		}
		//Unbind();
		//pVertexBuffer->Unind();
	}

}