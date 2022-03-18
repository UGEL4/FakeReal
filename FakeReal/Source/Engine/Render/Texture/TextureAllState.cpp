#include "TextureAllState.h"
#include "FRTexture.h"
#include "../../Graphic/Stream/Property.h"
#include "../RenderResource/RenderResource.h"
#include "../Renderer.h"

namespace FakeReal {
	IMPLEMENT_RTTI(TextureAllState, Object);
	IMPLEMENT_INITIAL_BEGIN(TextureAllState)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(TextureAllState, Object)
	REGISTER_PROPERTY(m_pTexture, "Texture", Property::F_SAVE_LOAD_CLONE)
	//REGISTER_PROPERTY(mSamplerDesc, "SamplerDesc", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pSamplerState, "SamplerState", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	TextureAllState::TextureAllState()
	{
		m_pTexture = nullptr;
		m_pSamplerState = nullptr;
		mbSRGB = false;
		std::cout << "构造TextureAllState:" << this << std::endl;
	}

	TextureAllState::TextureAllState(Texture* pTexture)
	{
		m_pTexture = pTexture;
		mbSRGB = false;
		m_pSamplerState = nullptr;
		std::cout << "构造TextureAllState:" << this << std::endl;
	}

	TextureAllState::~TextureAllState()
	{
		if (m_pTexture)
			delete m_pTexture;
		if (m_pSamplerState)
			delete m_pSamplerState;
		if (m_pID)
			delete m_pID;
		std::cout << "析构TextureAllState:" << this << std::endl;
	}

	bool TextureAllState::PostLoad(void * pData /*= nullptr*/)
	{
		TextureID* id = Renderer::CreateTextureID(m_pTexture, m_pSamplerState);
		m_pID = id;
		return m_pID != nullptr;
	}

	bool TextureAllState::PostClone(Object* pObjectSrc)
	{
		TextureID* id = Renderer::CreateTextureID(m_pTexture, m_pSamplerState);
		m_pID = id;
		return m_pID != nullptr;
	}

	void TextureAllState::SetSamplerState(SamplerState* pSamplerState)
	{
		if (m_pSamplerState == pSamplerState || !pSamplerState)
			return;

		if (m_pSamplerState)
			delete m_pSamplerState;

		m_pSamplerState = pSamplerState;
		mSamplerDesc = pSamplerState->GetSamplerDesc();
	}

	void TextureAllState::SetTexture(Texture* pTexture)
	{

	}

	void TextureAllState::SetTextureID(ResourceIdentifier* pID)
	{
		m_pID = pID;
	}

	void TextureAllState::BindTexture(unsigned int slot /*= 0*/)
	{
		if (m_pID)
		{
			TextureID* pID = (TextureID*)m_pID;
			pID->Bind(slot);
		}
	}

}