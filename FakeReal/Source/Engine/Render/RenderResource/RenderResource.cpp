#include "RenderResource.h"
#include "../RenderState/SamplerState.h"
#include "../Texture/FRTexture.h"
#include "../Texture/TextureCubeMap.h"
#include "../Buffer/VertexBuffer.h"
#include "../Buffer/IndexBuffer.h"
#include "../Buffer/BufferLayout.h"
#include <assert.h>
#include <glad/glad.h>

namespace FakeReal {
	/************************************************************************/
	/*							Texture								        */
	/************************************************************************/
	TextureID_GL::TextureID_GL()
	{
		std::cout << "构造TextureID_GL:" << this << std::endl;
	}

	TextureID_GL::~TextureID_GL()
	{
		glDeleteTextures(1, &mRendererID);
		std::cout << "析构TextureID_GL:" << this << std::endl;
	}

	void TextureID_GL::CreateRenderResource(const Texture* pTexture, const SamplerState* pSamplerState)
	{
		if (!pTexture || !pSamplerState)
			return;

		Texture::TEXTURE_TYPE TYPE = pTexture->GetTextureType();
		if (TYPE == Texture::TEXTURE_TYPE::TT_2D)
		{
			mTextureType = T2D;
			glGenTextures(1, &mRendererID);
			glBindTexture(GL_TEXTURE_2D, mRendererID);

			Texture::TEXTURE_FORMAT format = pTexture->GetTextureFormat();
			GLenum internalFormat = 0, dataFormat = 0;
			if (format == Texture::TEXTURE_FORMAT::TF_R8G8B8)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (format == Texture::TEXTURE_FORMAT::TF_R8G8B8A8)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else
				assert(0);

			const SamplerDesc& desc = pSamplerState->GetSamplerDesc();
			glTexStorage2D(GL_TEXTURE_2D, desc.AniLevel, internalFormat, pTexture->GetWidth(), pTexture->GetHeight());

			//U
			switch (desc.CoordMode_U)
			{
			case SamplerDesc::CoordinateMode::CM_CLAMP:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				break;
			case SamplerDesc::CoordinateMode::CM_REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				break;
			case SamplerDesc::CoordinateMode::CM_MIRRORED:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				break;
			case SamplerDesc::CoordinateMode::CM_BORDER:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameterIuiv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &desc.BorderColor[0]);
			}
			break;
			default:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				break;
			}
			//V
			switch (desc.CoordMode_V)
			{
			case SamplerDesc::CoordinateMode::CM_CLAMP:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;
			case SamplerDesc::CoordinateMode::CM_REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			case SamplerDesc::CoordinateMode::CM_MIRRORED:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
			case SamplerDesc::CoordinateMode::CM_BORDER:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				glTexParameterIuiv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &desc.BorderColor[0]);
			}
			break;
			default:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
			}
			//V
			switch (desc.CoordMode_W)
			{
			case SamplerDesc::CoordinateMode::CM_CLAMP:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				break;
			case SamplerDesc::CoordinateMode::CM_REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
				break;
			case SamplerDesc::CoordinateMode::CM_MIRRORED:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
				break;
			case SamplerDesc::CoordinateMode::CM_BORDER:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
				glTexParameterIuiv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &desc.BorderColor[0]);
			}
			break;
			default:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
				break;
			}

			if (desc.FliterMode_Mip == SamplerDesc::FliterMode::FM_ANISOTROPIC)
			{
				//TODO:这里还有bug
				glGenerateMipmap(GL_TEXTURE_2D);
				switch (desc.FliterMode_Min)
				{
				case SamplerDesc::FliterMode::FM_NEAREST:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					break;
				case SamplerDesc::FliterMode::FM_LINER:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					break;
				default:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					break;
				}
			}
			else
			{
				switch (desc.FliterMode_Min)
				{
				case SamplerDesc::FliterMode::FM_NEAREST:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					break;
				case SamplerDesc::FliterMode::FM_LINER:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					break;
				default:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					break;
				}
			}

			//放大纹理不会用到多级渐远纹理
			switch (desc.FliterMode_Mag)
			{
			case SamplerDesc::FliterMode::FM_NEAREST:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			case SamplerDesc::FliterMode::FM_LINER:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			default:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pTexture->GetWidth(), pTexture->GetHeight(), dataFormat, GL_UNSIGNED_BYTE, pTexture->GetData());
		}
		else if (TYPE == Texture::TEXTURE_TYPE::TT_CUBE)
		{
			mTextureType = TCUBE;
			glGenTextures(1, &mRendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererID);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			Texture::TEXTURE_FORMAT format = pTexture->GetTextureFormat();
			GLenum internalFormat = 0, dataFormat = 0;
			if (format == Texture::TEXTURE_FORMAT::TF_R8G8B8)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (format == Texture::TEXTURE_FORMAT::TF_R8G8B8A8)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else
				assert(0);

			TextureCubeMap* Temp = (TextureCubeMap*)pTexture;
			for (int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, pTexture->GetWidth(), pTexture->GetHeight(), 0, dataFormat, GL_UNSIGNED_BYTE, Temp->GetFaceBuffer(i));
			}
		}
	}

	void* TextureID_GL::GetRenderResource() const
	{
		return (void*)&mRendererID;
	}

	void TextureID_GL::Bind(unsigned int slot /*= 0*/)
	{
		if (mTextureType == T2D)
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, mRendererID);
		}
		else if (mTextureType == TCUBE)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererID);
		}
	}

	/************************************************************************/
	/*							VertexArray								    */
	/************************************************************************/

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

	VertexArrayID::VertexArrayID()
	{
		std::cout << "构造VertexArrayID:" << this << std::endl;
	}

	VertexArrayID::~VertexArrayID()
	{
		glDeleteVertexArrays(1, &mRendererID);
		std::cout << "析构VertexArrayID:" << this << std::endl;
	}

	void VertexArrayID::CreateRenderResource()
	{
		glGenVertexArrays(1, &mRendererID);
		Bind();
	}

	void* VertexArrayID::GetRenderResource() const
	{
		return (void*)&mRendererID;
	}

	void VertexArrayID::Bind() const
	{
		glBindVertexArray(mRendererID);
	}

	void VertexArrayID::Unbind() const
	{
		glBindVertexArray(0);
	}

	/************************************************************************/
	/*							VertexBuffer							    */
	/************************************************************************/

	VertexBufferID_GL::VertexBufferID_GL()
	{
		std::cout << "构造VertexBufferID_GL:" << this << std::endl;
	}

	VertexBufferID_GL::~VertexBufferID_GL()
	{
		glDeleteBuffers(1, &mRendererID);
		std::cout << "析构VertexBufferID_GL:" << this << std::endl;
	}

	void VertexBufferID_GL::CreateRenderResource(const VertexBuffer* pVB, bool BatchVertex)
	{
		glGenBuffers(1, &mRendererID);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		if (BatchVertex)
		{
			VertexArrayAttributeBatch(pVB);
		}
		else
		{
			VertexArrayAttributeInterleave(pVB);
		}
		
	}

	void* VertexBufferID_GL::GetRenderResource() const
	{
		return (void*)&mRendererID;
	}

	void VertexBufferID_GL::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	}

	void VertexBufferID_GL::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBufferID_GL::VertexArrayAttributeBatch(const VertexBuffer* pVB)
	{
		glBufferData(GL_ARRAY_BUFFER, pVB->GetBufferDataSize(), nullptr, GL_STATIC_DRAW);

		unsigned int index = 0;
		unsigned int BatchOffSet = 0;
		const auto& elements = pVB->GetBufferLayout().GetElements();
		for (const auto& e : elements)
		{
			DataBuffer* pData = nullptr;
			switch (e.m_Semantics)
			{
			case VertexFormat::VF_POSITION:
			{
				pData = pVB->GetPositionData(e.m_SemanticsIndex);
			}
			break;
			case VertexFormat::VF_NORMAL:
			{
				pData = pVB->GetNormalData(e.m_SemanticsIndex);
			}
			break;
			case VertexFormat::VF_TEXCOORD:
			{
				pData = pVB->GetTexCoordData(e.m_SemanticsIndex);
			}
			break;
			case VertexFormat::VF_BLENDWEIGHT:
			{
				pData = pVB->GetBlendWeightData();
			}
			break;
			case VertexFormat::VF_BLENDINDICES:
			{
				pData = pVB->GetBlendIndicesData();
			}
			break;
			default:
				assert(0);
				break;
			}
			glBufferSubData(GL_ARRAY_BUFFER, BatchOffSet, pData->GetDataSize(), pData->GetData());

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
					ShaderDataTypeSize(e.m_Type),
					(const void*)BatchOffSet
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
					ShaderDataTypeSize(e.m_Type),
					(const void*)BatchOffSet
				);
				index++;
				break;
			}
			case ShaderDataType::Mat3f:
			case ShaderDataType::Mat4f:
			{
				unsigned int count = e.ShaderDataTypeComponent();
				for (unsigned int i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(
						index,
						count,
						ShaderDataTypeToGLenum(e.m_Type),
						e.m_bNormalize ? GL_TRUE : GL_FALSE,
						ShaderDataTypeSize(e.m_Type),
						(const void*)(BatchOffSet + sizeof(float) * count * i)
					);
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				assert(0);
				break;
			}

			BatchOffSet += pData->GetDataSize();
		}
	}

	void VertexBufferID_GL::VertexArrayAttributeInterleave(const VertexBuffer* pVB)
	{
		unsigned char* data = pVB->GetBufferData();
		glBufferData(GL_ARRAY_BUFFER, pVB->GetBufferDataSize(), data, GL_STATIC_DRAW);

		unsigned int offser = 0;
		unsigned int index = 0;
		const auto& elements = pVB->GetBufferLayout().GetElements();
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
					pVB->GetBufferLayout().GetStride(),
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
					pVB->GetBufferLayout().GetStride(),
					(const void*)e.m_uiOffset
				);
				index++;
				break;
			}
			case ShaderDataType::Mat3f:
			case ShaderDataType::Mat4f:
			{
				unsigned int count = e.ShaderDataTypeComponent();
				for (unsigned int i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(
						index,
						count,
						ShaderDataTypeToGLenum(e.m_Type),
						e.m_bNormalize ? GL_TRUE : GL_FALSE,
						pVB->GetBufferLayout().GetStride(),
						(const void*)(e.m_uiOffset + sizeof(float) * count * i)
					);
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

		delete[] data;
	}

	/************************************************************************/
	/*							IndexBuffer							        */
	/************************************************************************/
	IndexBufferID_GL::IndexBufferID_GL()
	{
		std::cout << "构造IndexBufferID_GL:" << this << std::endl;
	}

	IndexBufferID_GL::~IndexBufferID_GL()
	{
		glDeleteBuffers(1, &mRendererID);
		std::cout << "析构IndexBufferID_GL:" << this << std::endl;
	}

	void IndexBufferID_GL::CreateRenderResource(const IndexBuffer* pData)
	{
		glGenBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pData->GetIndexCount() * sizeof(uint32_t), pData->GetIndexData()->GetData(), GL_STATIC_DRAW);
	}

	void* IndexBufferID_GL::GetRenderResource() const
	{
		return (void*)&mRendererID;
	}

	void IndexBufferID_GL::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}

	void IndexBufferID_GL::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}