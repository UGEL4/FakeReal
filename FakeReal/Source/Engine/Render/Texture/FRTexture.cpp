#include "FRTexture.h"
#include "../Renderer.h"
#include <assert.h>
#include "../../Graphic/Stream/Property.h"
#include <stb_image.h>
#include <iostream>

namespace FakeReal {
	//Texture
	IMPLEMENT_RTTI_NO_CREATE_FUNCTION(Texture, Object);
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(Texture)
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END;
	BEGIN_ADD_PROPERTY(Texture, Object)
	REGISTER_PROPERTY(mWidth, "Width", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mHeight, "Height", Property::F_SAVE_LOAD_CLONE)
	REGISTER_ENUM_PROPERTY(mTextureFormat, "TextureFormat", "TextureFormat", Property::F_SAVE_LOAD_CLONE)
	REGISTER_DATA_PROPERTY(m_pDataBuffer, mDataBuffSize, "DataBuffer")
	END_ADD_PROPERTY

	//Texture2D
	IMPLEMENT_RTTI(Texture2D, Texture);
	IMPLEMENT_INITIAL_BEGIN(Texture2D)
	IMPLEMENT_INITIAL_END
	BEGIN_ADD_PROPERTY(Texture2D, Texture)
	REGISTER_PROPERTY(mPath, "Path", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	Texture2D::Texture2D(const std::string& file, unsigned int width, unsigned int height, TEXTURE_FORMAT format)
	{
		mPath = file;
		mWidth = width;
		mHeight = width;
		mTextureFormat = format;
		m_pDataBuffer = nullptr;
		mDataBuffSize = 0;
		std::cout << "构造Texture2D:" << this << std::endl;
	}

	Texture2D::Texture2D()
	{
		std::cout << "构造Texture2D:" << this << std::endl;
	}

	Texture2D::~Texture2D()
	{
		std::cout << "析构Texture2D:" << this << std::endl;
	}

	LoadImageData Texture2D::LoadImage(const std::string& file, bool flip_v /*= true*/)
	{
		LoadImageData LoadData;
		LoadData.data = nullptr;
		LoadData.format = Texture::TEXTURE_FORMAT::TF_R8G8B8;
		int channels;
		stbi_set_flip_vertically_on_load(flip_v);

		LoadData.dataSize = 0;

		stbi_uc* data = nullptr;
		data = stbi_load(file.data(), &LoadData.width, &LoadData.height, &channels, 0);
		if (data)
		{
			if (channels == 4)
				LoadData.format = Texture::TEXTURE_FORMAT::TF_R8G8B8A8;
			LoadData.data = data;

			//PNG
			LoadData.dataSize = LoadData.width * LoadData.height * channels;
		}
		else
		{
			std::cout << "stbi_load result: data is nullptr, file : " << file << std::endl;
		}
		return LoadData;
	}

}


