#pragma once

#include "FRTexture.h"

namespace FakeReal {
	class FR_ENGINE_API TextureCubeMap : public Texture
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		enum
		{
			FACE_RIGHT,
			FACE_LEFT,
			FACE_TOP,
			FACE_BOTTOM,
			FACE_BACK,
			FACE_FRONT,
			FACE_MAX
		};

		TextureCubeMap(Texture2D* pTextures[FACE_MAX]);
		~TextureCubeMap();

		virtual const std::string& GetPath() const override { return ""; }
		virtual TextureType GetTextureType() const override { return Texture::TEXTURE_TYPE::TT_CUBE; }

		unsigned char* GetFaceBuffer(unsigned int face) const;

	private:
		TextureCubeMap() {}
	};
	FR_TYPE_MARCO(TextureCubeMap)
}