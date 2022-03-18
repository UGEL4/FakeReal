#pragma once
#include "../../Core/CoreMarco.h"
#include "../Object.h"
#include "Actor/Actor.h"
#include "Scene/Scene.h"
namespace FakeReal {
	class Node;
	class FR_ENGINE_API SceneMap : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL

	public:
		SceneMap();
		SceneMap(const std::string& name);
		~SceneMap();

		const std::string& GetSceneName() const
		{
			return mName;
		}
		std::vector<Actor*>& GetActorArray()
		{
			return mActorArray;
		}

		void OnDestroy();

		void AddActor(Actor* pActor);
		//只移除对象，不删除
		void RemoveActor(Actor* pActor);
		//void DeleteActor(Actor* pActor);
		void RemoveComponent(Node* pComponent);

		friend class World;

		Scene* GetScene() const { return m_pScene; }
	protected:
		Scene* m_pScene;
		std::string mName;
		std::vector<Actor*> mActorArray;
	};
	FR_TYPE_MARCO(SceneMap)
}