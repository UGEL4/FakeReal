#pragma once
#include "../../../Core/CoreMarco.h"
#include "Scene.h"
/*
	World负责创建实体和地图（SceneMap）、更新实体、给实体发消息。SceneMap存放自己拥有的实体，并拥有一个Scene指针，Scene则是管理Actor的组件。
	世界（地图（实体（组件）））<=== = > 场景管理器（场景（所有实体的组件））
*/
namespace FakeReal {
	class FR_ENGINE_API SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		static SceneManager* ms_pSceneManager;

		void Update(float appTime);
		void AddScene(Scene* pScene);
		void DeleteScene(Scene* pScene);
		void DeleteScene(unsigned int index);
		void ClearAllScene();
		Scene* GetScene(unsigned int index) const;
		unsigned int GetSceneNum() const;
	protected:
		std::vector<Scene*> mSceneArray;
	};
}