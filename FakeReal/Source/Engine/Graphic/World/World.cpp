#include "World.h"
#include "../Stream/Property.h"
#include "../Stream/Stream.h"
#include "../Resource/Resource.h"
#include "../Resource/ResourceManager.h"
#include "Actor/StaticActor.h"

namespace FakeReal {
	World* World::ms_pWorld = nullptr;

	IMPLEMENT_RTTI(World, Object);
	IMPLEMENT_INITIAL_BEGIN(World)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(World, Object)
	REGISTER_PROPERTY(mSceneMapArray, "SceneMapArray", Property::F_REFLECT_NAME)
	END_ADD_PROPERTY

	World::World()
	{
		ms_pWorld = this;
		mActorArray.clear();
		mSceneMapArray.clear();
		CreateSceneMap("Default");
		std::cout << "构造World:" << this << std::endl;
	}

	World::~World()
	{
		for (size_t i = 0; i < mActorArray.size(); i++)
		{
			DestroyActor(mActorArray[i]);
		}

		for (size_t i = 0; i < mSceneMapArray.size(); i++)
		{
			if (mSceneMapArray[i])
			{
				mSceneMapArray[i]->OnDestroy();
				delete mSceneMapArray[i];
			}
		}
		mActorArray.clear();
		mSceneMapArray.clear();
		std::cout << "析构World:" << this << std::endl;
	}


	SceneMap* World::CreateSceneMap(const std::string& name)
	{
		size_t num = mSceneMapArray.size();
		for (size_t i = 0; i < num; i++)
		{
			if (name == mSceneMapArray[i]->GetSceneName())
			{
				return nullptr;
			}
		}
		SceneMap* p = new SceneMap(name);
		mSceneMapArray.emplace_back(p);
		return p;
	}

	SceneMap* World::GetSceneMap(const std::string& name) const
	{
		size_t num = mSceneMapArray.size();
		for (size_t i = 0; i < num; i++)
		{
			if (name == mSceneMapArray[i]->GetSceneName())
			{
				return mSceneMapArray[i];
			}
		}
		return nullptr;
	}

	SceneMap* World::GetSceneMap(unsigned int uiIndex) const
	{
		if (uiIndex < 0 || uiIndex >= mSceneMapArray.size())
		{
			return nullptr;
		}
		return mSceneMapArray[0];
	}

	Actor* World::CreateActor(const std::string& actorPath, const glm::vec3& pos, const glm::vec3& rotate, const glm::vec3& scale, SceneMap* pSceneMap)
	{
		Actor* pActor = nullptr;
		ResourceProxyBase* pRes = ResourceManager::LoadResource(actorPath);
		if (pRes)
		{
			unsigned int ResType = pRes->GetResourceType();
			if (ResType == Resource::RT_STATIC_MODEL)
			{
				pActor = CreateActor<StaticActor>(pos, rotate, scale, pSceneMap);
				if (pActor)
				{
					((StaticActor*)pActor)->SetStaticMeshResource((StaticMeshNodeR*)pRes);
				}
			}
		}
		return pActor;
	}

	void World::Update(float appTime)
	{
		for (Actor* p : mActorArray)
		{
			p->Update(appTime);
		}
	}

	void World::ProcessInput()
	{
		for (Actor* p : mActorArray)
		{
			p->ProcessInput();
		}
	}

	void World::DestroyActor(Actor* pActor)
	{
		//OnDestroy把Actor从SceneMap中移除同时组件也从Scene中移除
		pActor->OnDestroy();
		DeleteActor(pActor);
	}

	void World::AddSceneMap(SceneMap* pSceneMap)
	{
		for (size_t i = 0; i < pSceneMap->mActorArray.size(); i++)
		{
			AddActor(pSceneMap->mActorArray[i]);
		}
		mSceneMapArray.emplace_back(pSceneMap);
	}

	void World::DestroySceneMap(const std::string& name)
	{
		//第一个地图不能删除
		size_t num = mSceneMapArray.size();
		for (size_t i = 1; i < num; i++)
		{
			if (name == mSceneMapArray[i]->GetSceneName())
			{
				for (size_t j = 0; j < mActorArray.size();)
				{
					if (mActorArray[j]->GetSceneMap() == mSceneMapArray[i])
					{
						DestroyActor(mActorArray[j]);
					}
					else
					{
						j++;
					}
				}
				mSceneMapArray[i]->OnDestroy();
				delete mSceneMapArray[i];
				mSceneMapArray.erase(mSceneMapArray.begin() + i);
				return;
			}
		}
	}

	void World::AddActor(Actor* pActor)
	{
		if (pActor)
		{
			mActorArray.emplace_back(pActor);
		}
	}

	void World::DeleteActor(Actor* pActor)
	{
		size_t num = mSceneMapArray.size();
		for (size_t i = 0; i < num; i++)
		{
			if (pActor == mActorArray[i])
			{
				delete mActorArray[i];
				mActorArray.erase(mActorArray.begin() + i);
				return;
			}
		}
	}

}