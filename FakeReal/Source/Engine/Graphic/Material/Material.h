#pragma once

#include "../../Core/CoreMarco.h"
#include "../Object.h"
#include "../Resource/Resource.h"

namespace FakeReal {
	class MaterialInterface : public Object, public Resource
	{
		DECLARE_RTTI
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		MaterialInterface() {}
		virtual ~MaterialInterface() = 0 {}

		virtual unsigned int GetResourceType() const
		{
			return RT_MATERIAL;
		}
	};
	FR_TYPE_MARCO(MaterialInterface);

	//基于文件的材质
	class Shader;
	class TextureAllState;
	DECLARE_Proxy(TextureAllState)
	class CustomMaterial : public MaterialInterface
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		CustomMaterial(const std::string& ResourceName, const std::string& ShaderFile);
		~CustomMaterial();

		virtual bool PostLoad(void * pData = nullptr) override;
		virtual bool PostClone(Object* pObjectSrc) override;

		void SetTexture(TextureAllStateRPtr tex);
		void UseMaterial();
	protected:
		CustomMaterial();

	private:
		std::string mShaderName;
		Shader* m_pShader;
		std::vector<TextureAllStateRPtr> mTexAllStateResource;
	};
}
