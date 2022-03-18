#pragma once

#include "../../Graphic/Object.h"
#include "../../Graphic/Resource/Resource.h"
#include "../RenderState/SamplerState.h"
#include "../../Core/CoreMarco.h"

namespace FakeReal {
	class Texture;
	class ResourceIdentifier;
	class FR_ENGINE_API TextureAllState : public Object, public Resource
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		TextureAllState();
		TextureAllState(Texture* pTexture);
		virtual ~TextureAllState();

		virtual unsigned int GetResourceType() const override { return RT_TEXTURE; }
		virtual bool PostLoad(void * pData = nullptr) override;
		virtual bool PostClone(Object* pObjectSrc) override;

		void SetSamplerState(SamplerState* pSamplerState);
		SamplerState* GetSamplerState() const { return m_pSamplerState; }
		bool GetSRGB() const { return mbSRGB; }

		void SetTexture(Texture* pTexture);
		Texture* GetTexture() const { return m_pTexture; }
		void SetTextureID(ResourceIdentifier* pID);
		ResourceIdentifier* GetTextureID() const { return m_pID; }
		void BindTexture(unsigned int slot = 0);

		Texture* m_pTexture;
	protected:
		SamplerDesc mSamplerDesc;
		SamplerState* m_pSamplerState;
		ResourceIdentifier* m_pID;//Œ∆¿Ì‰÷»æ◊ ‘¥
		bool mbSRGB;
	};
	FR_TYPE_MARCO(TextureAllState)
	DECLARE_Proxy(TextureAllState)
}