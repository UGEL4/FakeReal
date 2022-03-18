#include "Material.h"
#include "../Stream/Property.h"
#include "../../Render/Shader/Shader.h"
#include "../../Render/Texture/TextureAllState.h"

namespace FakeReal {
	IMPLEMENT_RTTI_NO_CREATE_FUNCTION(MaterialInterface, Object);
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(MaterialInterface)
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END;
	BEGIN_ADD_PROPERTY(MaterialInterface, Object)
	END_ADD_PROPERTY;

	IMPLEMENT_RTTI(CustomMaterial, MaterialInterface);
	IMPLEMENT_INITIAL_BEGIN(CustomMaterial)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(CustomMaterial, MaterialInterface)
	REGISTER_PROPERTY(mShaderName, "ShaderKey", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mTexAllStateResource, "Textures", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;

	CustomMaterial::CustomMaterial(const std::string& ResourceName, const std::string& ShaderFile)
	{
		mShaderName = ResourceName;
		if (ShaderLibrary::gs_pShaderLib->IsExist(mShaderName))
		{
			m_pShader = ShaderLibrary::gs_pShaderLib->Get(mShaderName);
		}
		else
		{
			ShaderLibrary::gs_pShaderLib->LoadShader(ShaderFile);
			m_pShader = ShaderLibrary::gs_pShaderLib->Get(mShaderName);
		}
	}

	CustomMaterial::CustomMaterial()
	{

	}

	CustomMaterial::~CustomMaterial()
	{

	}

	bool CustomMaterial::PostLoad(void * pData /*= nullptr*/)
	{
		m_pShader = ShaderLibrary::gs_pShaderLib->Get(mShaderName);
		return true;
	}

	bool CustomMaterial::PostClone(Object* pObjectSrc)
	{
		m_pShader = ShaderLibrary::gs_pShaderLib->Get(mShaderName);
		return true;
	}

	void CustomMaterial::SetTexture(TextureAllStateRPtr tex)
	{
		if (!tex)
			return;
		for (TextureAllStateRPtr p : mTexAllStateResource)
		{
			if (tex == p)
				return;
		}
		mTexAllStateResource.emplace_back(tex);
	}

	void CustomMaterial::UseMaterial()
	{
		if (m_pShader)
		{
			m_pShader->Bind();
			for (size_t i = 0; i < mTexAllStateResource.size(); i++)
			{
				m_pShader->SetInt(("TexSlot_" + std::to_string(i)).c_str(), i);
				mTexAllStateResource[i]->GetResource()->BindTexture(i);
			}
		}
	}

}
