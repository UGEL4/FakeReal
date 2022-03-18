#include "Scene.h"
#include "../../Node/Node.h"
#include "../../Stream/Property.h"
#include "SceneManager.h"

namespace FakeReal {
	IMPLEMENT_RTTI(Scene, Object);
	IMPLEMENT_INITIAL_BEGIN(Scene)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(Scene, Object)
	REGISTER_PROPERTY(mNodeArray, "Object Components", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	Scene::Scene()
	{
		mNodeArray.clear();
		SceneManager::ms_pSceneManager->AddScene(this);
		std::cout << "¹¹ÔìScene:" << this << std::endl;
	}

	Scene::~Scene()
	{
		mNodeArray.clear();
		SceneManager::ms_pSceneManager->DeleteScene(this);
		std::cout << "Îö¹¹Scene:" << this << std::endl;
	}

	void Scene::Update(float appTime)
	{
		for (Node* p : mNodeArray)
		{
			p->UpdateAll(appTime);
		}
	}

	void Scene::AddComponent(Node* pComponent)
	{
		mNodeArray.emplace_back(pComponent);
	}

	void Scene::RemoveComponent(Node* pComponent)
	{
		auto iter = std::find(mNodeArray.begin(), mNodeArray.end(), pComponent);
		if (iter != mNodeArray.end())
		{
			mNodeArray.erase(iter);
		}
	}

	void Scene::RemoveAllComponent()
	{
		mNodeArray.clear();
	}

}