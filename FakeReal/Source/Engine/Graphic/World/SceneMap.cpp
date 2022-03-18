#include "SceneMap.h"
#include "../Stream/Property.h"

namespace FakeReal {
	IMPLEMENT_RTTI(SceneMap, Object);
	IMPLEMENT_INITIAL_BEGIN(SceneMap)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(SceneMap, Object)
	REGISTER_PROPERTY(m_pScene, "Scene", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mName, "Scene Name", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mActorArray, "Actors", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	SceneMap::SceneMap()
	{
		mActorArray.clear();
		std::cout << "构造SceneMap:" << this << std::endl;
	}

	SceneMap::SceneMap(const std::string& name)
	{
		m_pScene = new Scene;
		mName = name;
		mActorArray.clear();
		std::cout << "构造SceneMap:" << this << std::endl;
	}

	SceneMap::~SceneMap()
	{
		if (m_pScene)
		{
			delete m_pScene;
		}
		m_pScene = nullptr;
		mActorArray.clear();
		std::cout << "析构SceneMap:" << this << std::endl;
	}

	void SceneMap::OnDestroy()
	{
		if (m_pScene)
		{
			delete m_pScene;
		}
		m_pScene = nullptr;
	}

	void SceneMap::AddActor(Actor* pActor)
	{
		if (pActor)
		{
			size_t num = mActorArray.size();
			for (size_t i = 0; i < num; i++)
			{
				if (pActor == mActorArray[i])
				{
					pActor->AddToSceneMap(this);
					return;
				}
			}
			if (pActor->GetSceneMap())
			{
				pActor->GetSceneMap()->RemoveActor(pActor);
			}
			
			mActorArray.emplace_back(pActor);
			m_pScene->AddComponent(pActor->GetRootNode());
			pActor->AddToSceneMap(this);
		}
	}

	void SceneMap::RemoveActor(Actor* pActor)
	{
		size_t num = mActorArray.size();
		for (size_t i = 0; i < num; i++)
		{
			if (pActor == mActorArray[i])
			{
				if (pActor->GetOwner())
				{
					pActor->GetOwner()->RemoveChildActor(pActor);
				}
				else
				{
					m_pScene->RemoveComponent(pActor->GetRootNode());
				}
				pActor->AddToSceneMap(nullptr);
				mActorArray.erase(mActorArray.begin() + i);
				return;
			}
		}
	}

	void SceneMap::RemoveComponent(Node* pComponent)
	{

	}

}