#pragma once

#include "../../Core/CoreMarco.h"
#include "../Object.h"
#include "Actor/Actor.h"
#include "SceneMap.h"
namespace FakeReal {
	class FR_ENGINE_API World : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		World();
		~World();
		static World* ms_pWorld;

		SceneMap* CreateSceneMap(const std::string& name);
		SceneMap* GetSceneMap(const std::string& name) const;
		SceneMap* GetSceneMap(unsigned int uiIndex) const;
		Actor* CreateActor(const std::string& actorPath,
			const glm::vec3& pos = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& rotate = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			SceneMap* pSceneMap = nullptr);

		template<class T>
		Actor* CreateActor(const glm::vec3& pos = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& rotate = { 0.0f, 0.0f, 0.0f },
			const glm::vec3& scale = { 1.0f, 1.0f, 1.0f },
			SceneMap* pSceneMap = nullptr);

		void Update(float appTime);
		void ProcessInput();

		friend class SceneManager;

		void DestroyActor(Actor* pActor);
	protected:
		void AddSceneMap(SceneMap* pSceneMap);
		void DestroySceneMap(const std::string& name);
		void AddActor(Actor* pActor);
		void DeleteActor(Actor* pActor);
	protected:
		std::vector<Actor*> mActorArray;//所有的对象，包括对象下的子对象
		std::vector<SceneMap*> mSceneMapArray;
	};
	FR_TYPE_MARCO(World)

	template<class T>
	Actor* World::CreateActor(const glm::vec3& pos, const glm::vec3& rotate, const glm::vec3& scale, SceneMap* pSceneMap)
	{
		if (!T::ms_Type.IsDerived(Actor::ms_Type))
			return nullptr;

		Actor* pActor = nullptr;
		pActor = (Actor*)Object::GetInstance<T>();
		pActor->CreateDefaultComponent();
		pActor->SetWorldPos(pos);
		pActor->SetWorldRotation(rotate);
		pActor->SetWorldScale(scale);
		if (pSceneMap)
		{
			pSceneMap->AddActor(pActor);
		}
		else
		{
			mSceneMapArray[0]->AddActor(pActor);
		}
		AddActor(pActor);
		return pActor;
	}
}