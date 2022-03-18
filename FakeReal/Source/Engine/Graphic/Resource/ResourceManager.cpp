#include "ResourceManager.h"
#include "Resource.h"
#include "../Node/Model/StaticMeshNode.h"
#include "../Node/Model/SkeletonMeshNode.h"
#include "../Stream/Stream.h"
#include "../../Render/Texture/TextureAllState.h"

namespace FakeReal {
	StaticMeshNodeRPtr ResourceManager::ms_pDefaultStaticMeshNodeResource = nullptr;
	SkeletonMeshNodeRPtr ResourceManager::ms_pDefaultSkeletonMeshNodeResource = nullptr;
	//Texture2DRPtr ResourceManager::ms_pDefaultTexture2DResource = nullptr;

	IMPLEMENT_INITIAL_ONLY_BEGIN(ResourceManager)
	ADD_INITIAL_FUNCTION(InitialDefaultState)
	ADD_TERMINATE_FUNCTION(TerminalDefaultState)
	IMPLEMENT_INITIAL_ONLY_END

	bool ResourceManager::InitialDefaultState()
	{
		return true;
	}

	bool ResourceManager::TerminalDefaultState()
	{
		delete ms_pDefaultStaticMeshNodeResource;
		delete ms_pDefaultSkeletonMeshNodeResource;
		return true;
	}

	void ResourceManager::LoadDefaultResource()
	{
		//ms_pDefaultTexture2DResource = Texture2DR::Create((Texture2D*)Texture2D::GetDefault());
		ms_pDefaultStaticMeshNodeResource = StaticMeshNodeR::Create((StaticMeshNode*)StaticMeshNode::GetDefault());
		ms_pDefaultSkeletonMeshNodeResource = SkeletonMeshNodeR::Create((SkeletonMeshNode*)SkeletonMeshNode::GetDefault());
	}

	bool ResourceManager::NewSaveAnimation(Animation* pAnimation, const char* file)
	{
		/*if (!pAnimation)
		{
			return false;
		}
		Stream SaveStream;
		SaveStream.SetStreamFlag(Stream::AT_REGISTER);
		SaveStream.ArchiveAll(pAnimation);
		eastl::string Path("Resources/Character/Animations/");
		Path += file;
		Path += "." + Resource::GetFileSuffix(Resource::RT_ANIMATION) + ".assert";
		SaveStream.NewSave(Path.data());*/
		return true;
	}

	Animation* ResourceManager::NewLoadAnimation(const char* file)
	{
		if (!file)
		{
			return nullptr;
		}

		Animation* pAnimation = nullptr;
		return pAnimation;
	}

	SkeletalMeshModel* ResourceManager::NewLoadSkeletalMeshModel(const char* file)
	{
		if (!file)
		{
			return nullptr;
		}
		SkeletalMeshModel* pModel = nullptr;
		return pModel;
	}

	bool ResourceManager::NewSaveSkeletalMeshModel(SkeletalMeshModel* pModel, const char* file)
	{
		if (!pModel)
		{
			return false;
		}
		return true;
	}

	Skeleton* ResourceManager::NewLoadSkeleton(const char* file)
	{
		if (!file)
		{
			return nullptr;
		}
	
		Skeleton* pSkeleton = nullptr;
		return pSkeleton;
	}

	bool ResourceManager::NewSaveSkeleton(Skeleton* pSkeleton, const char* file)
	{
		if (!pSkeleton)
		{
			return false;
		}
		return true;
	}

	TextureAllStateR* ResourceManager::LoadTexture(const std::string& file)
	{
		Stream LoadStream;
		LoadStream.NewLoad(file.data());

		TextureAllState* pTex = nullptr;
		pTex = (TextureAllState*)LoadStream.GetObjectByRtti(TextureAllState::ms_Type);
		if (!pTex)
			return nullptr;

		TextureAllStateR* pRes = new TextureAllStateR;
		pRes->SetResourceName(file);
		pRes->SetNewResource(pTex);
		pRes->Loaded();

		return pRes;
	}

	StaticMeshNodeR* ResourceManager::LoadStaticModel(const std::string& file)
	{
		Stream LoadStream;
		LoadStream.NewLoad(file.data());

		StaticMeshNode* pMode = (StaticMeshNode*)LoadStream.GetObjectByRtti(StaticMeshNode::ms_Type);
		if (!pMode)
			return nullptr;

		StaticMeshNodeR* pRes = new StaticMeshNodeR;
		pRes->SetResourceName(file);
		pRes->SetNewResource(pMode);
		pRes->Loaded();

		return pRes;
	}

	ResourceProxyBase* ResourceManager::LoadResource(const std::string& name)
	{
		ResourceProxyBase* pRes = nullptr;

		/*找出后缀例如xx.animation.assert中的animation*/
		auto lastDot = name.rfind('.');
		lastDot = lastDot == std::string::npos ? 0 : lastDot;
		auto dot = name.rfind('.', lastDot - 1);
		dot = dot == std::string::npos ? 0 : dot + 1;
		auto count = lastDot - dot;
		std::string suffix = name.substr(dot, count);

		if (suffix == Resource::GetFileSuffix(Resource::RT_TEXTURE))
		{
			pRes = LoadTexture(name);
		}
		else if (suffix == Resource::GetFileSuffix(Resource::RT_STATIC_MODEL))
		{
			pRes = LoadStaticModel(name);
		}

		return pRes;
	}

}
