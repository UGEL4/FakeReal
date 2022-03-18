#include "TextureCubeMap.h"
#include "../../Graphic/Stream/Property.h"
#include <assert.h>
namespace FakeReal {
	IMPLEMENT_RTTI(TextureCubeMap, Object);
	IMPLEMENT_INITIAL_BEGIN(TextureCubeMap)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(TextureCubeMap, Object)
	END_ADD_PROPERTY

	TextureCubeMap::TextureCubeMap(Texture2D* pTextures[FACE_MAX])
	{
		bool bFind = false;
		unsigned int width = 0;
		unsigned int height = 0;
		TEXTURE_FORMAT format = TEXTURE_FORMAT::TF_R8G8B8;

		for (unsigned int i = 0; i < FACE_MAX; i++)
		{
			if (!bFind)
			{
				if (pTextures[i])
				{
					width = pTextures[i]->GetWidth();
					height = pTextures[i]->GetHeight();
					format = pTextures[i]->GetTextureFormat();
					bFind = true;
				}
			}
			else
			{
				if (width != pTextures[i]->GetWidth() ||
					height != pTextures[i]->GetHeight() ||
					format != pTextures[i]->GetTextureFormat())
				{
					assert(0);
				}
			}
		}

		if (!bFind)
		{
			assert(0);
		}

		unsigned int channel = 0;
		if (format == TEXTURE_FORMAT::TF_R8G8B8)
			channel = 3;
		else if (format == TEXTURE_FORMAT::TF_R8G8B8A8)
			channel = 4;
		mTextureFormat = format;
		mWidth = width;
		mHeight = height;
		mDataBuffSize = width * height * channel * 6;

		m_pDataBuffer = new unsigned char[mDataBuffSize];
		if (!m_pDataBuffer)
		{
			assert(0);
		}

		unsigned int FaceSize = width * height * channel;
		for (unsigned int i = 0; i < FACE_MAX; i++)
		{
			memcpy_s(GetFaceBuffer(i), FaceSize, pTextures[i]->GetData(), FaceSize);
		}
	}

	TextureCubeMap::~TextureCubeMap()
	{

	}

	unsigned char* TextureCubeMap::GetFaceBuffer(unsigned int face) const
	{
		if (face >= FACE_MAX)
			return nullptr;

		unsigned int channel = 0;
		if (mTextureFormat == TEXTURE_FORMAT::TF_R8G8B8)
			channel = 3;
		else if (mTextureFormat == TEXTURE_FORMAT::TF_R8G8B8A8)
			channel = 4;
		return GetData() + mWidth * mHeight * channel * face;
	}

}