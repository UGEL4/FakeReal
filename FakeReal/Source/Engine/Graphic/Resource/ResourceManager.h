#pragma once
#include "Resource.h"
#include "../../Core/CoreMarco.h"
#include "../../Core/EngineMain.h"
namespace FakeReal {

	class Animation;
	class SkeletalMeshModel;
	class Skeleton;
	class Resource;
	class ResourceProxyBase;
	class StaticMeshNode;
	DECLARE_Proxy(StaticMeshNode);
	class TextureAllState;
	DECLARE_Proxy(TextureAllState);
	class SkeletonMeshNode;
	DECLARE_Proxy(SkeletonMeshNode);
	class FR_ENGINE_API ResourceManager
	{
		DECLARE_INITIAL_ONLY
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static void LoadDefaultResource();

		/*Animation*/
		static bool NewSaveAnimation(Animation* pAnimation, const char* file);
		static Animation* NewLoadAnimation(const char* file);
		/*SkeletalMeshModel*/
		static SkeletalMeshModel* NewLoadSkeletalMeshModel(const char* file);
		static bool NewSaveSkeletalMeshModel(SkeletalMeshModel* pModel, const char* file);
		/*Skeleton*/
		static Skeleton* NewLoadSkeleton(const char* file);
		static bool NewSaveSkeleton(Skeleton* pSkeleton, const char* file);
		/*texture*/
		static TextureAllStateR* LoadTexture(const std::string& file);
		/*StaticMeshModel*/
		static StaticMeshNodeR* LoadStaticModel(const std::string& file);

		static ResourceProxyBase* LoadResource(const std::string& name);

	public:
		static StaticMeshNodeRPtr ms_pDefaultStaticMeshNodeResource;
		static SkeletonMeshNodeRPtr ms_pDefaultSkeletonMeshNodeResource;
		//static Texture2DRPtr ms_pDefaultTexture2DResource;
	};
}
