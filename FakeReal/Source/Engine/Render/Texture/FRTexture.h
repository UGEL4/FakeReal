#pragma once
#include <string>
#include <memory>
#include "../../Graphic/Resource/Resource.h"
#include "../../Graphic/Object.h"
#include "../../Core/CoreMarco.h"
#include "../../Core/FRNVTTCompression.h"

namespace FakeReal {

	class FR_ENGINE_API Texture : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL_NO_CLASS_FACTORY

	public:
		typedef enum class TEXTURE_TYPE : unsigned short//贴图类型
		{
			TT_1D,
			TT_2D,
			TT_CUBE,//立方体贴图
			TT_MAX
		}TextureType;

		typedef enum TEXTURE_FORMAT : unsigned short
		{
			TF_R8G8B8,
			TF_R8G8B8A8,
			TF_D24S8,
			TF_MAX
		}TextureFormat;
	public:
		Texture()
		{
			mWidth = 0;
			mHeight = 0;
			mTextureFormat = TEXTURE_FORMAT::TF_R8G8B8;
			m_pDataBuffer = nullptr;
			mDataBuffSize = 0;
		}
		virtual ~Texture()
		{
			if (m_pDataBuffer)
				delete[] m_pDataBuffer;
		}

		virtual const std::string& GetPath() const = 0;
		virtual TextureType GetTextureType() const = 0;

		TextureFormat GetTextureFormat() const { return mTextureFormat; }
		unsigned int GetWidth() const { return mWidth; }
		unsigned int GetHeight() const { return mHeight; }
		unsigned int GetTextureDataSize() const { return mDataBuffSize; }
		unsigned char* GetData() const { return m_pDataBuffer; }

		void SetData(void* data, unsigned int uiSize)
		{
			m_pDataBuffer = (unsigned char*)data;
			mDataBuffSize = uiSize;
		}

		bool HasAlpha() const
		{
			if (mTextureFormat == TEXTURE_FORMAT::TF_R8G8B8A8)
				return true;
			else
				return false;
		}

	protected:
		unsigned int mWidth;
		unsigned int mHeight;
		TextureFormat mTextureFormat;
		unsigned char* m_pDataBuffer;//贴图数据
		unsigned int mDataBuffSize;//数据大小

	private:


	};
	FR_TYPE_MARCO(Texture)


	struct LoadImageData
	{
		unsigned char* data;
		unsigned int dataSize;
		int width;
		int height;
		Texture::TEXTURE_FORMAT format;
	};

	class FR_ENGINE_API Texture2D : public Texture
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		Texture2D();
		Texture2D(const std::string& file, unsigned int width, unsigned int height, TEXTURE_FORMAT format);
		Texture2D(const std::string& file, bool compress, bool flip);
		virtual ~Texture2D();

		virtual const std::string& GetPath() const override { return mPath; }
		virtual TextureType GetTextureType() const override { return TEXTURE_TYPE::TT_2D; }

		static LoadImageData LoadImage(const std::string& file, bool flip_v = true);
		static void LoadImageCompressAndSaveDDS(const std::string& file, const std::string& outFile, FRNVTTCompression::CompressFormat format, bool forceUseCpu = true, bool flip_v = true);

	private:
		static void SaveDDS(const std::string& file, const char* data, unsigned int size);
		std::string mPath;
		bool m_bIsCompressed;

	};
	FR_TYPE_MARCO(Texture2D);
}
