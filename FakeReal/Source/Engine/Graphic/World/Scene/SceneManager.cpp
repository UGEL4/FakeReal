#include "SceneManager.h"

namespace FakeReal {
	SceneManager* SceneManager::ms_pSceneManager = nullptr;

	SceneManager::SceneManager()
	{
		ms_pSceneManager = this;
		mSceneArray.clear();
		std::cout << "¹¹ÔìSceneManager:" << this << std::endl;
	}

	SceneManager::~SceneManager()
	{
		std::cout << "Îö¹¹SceneManager:" << this << std::endl;
	}

	void SceneManager::Update(float appTime)
	{
		for (Scene* pScene : mSceneArray)
		{
			pScene->Update(appTime);
		}
	}

	void SceneManager::AddScene(Scene* pScene)
	{
		if (pScene)
		{
			unsigned int uiSceneNum = mSceneArray.size();
			for (unsigned int i = 0; i < uiSceneNum; i++)
			{
				if (pScene == mSceneArray[i])
				{
					return;
				}
			}
			mSceneArray.emplace_back(pScene);
		}
	}

	void SceneManager::DeleteScene(Scene* pScene)
	{
		if (pScene)
		{
			unsigned int uiSceneNum = mSceneArray.size();
			for (unsigned int i = 0; i < uiSceneNum; i++)
			{
				if (pScene == mSceneArray[i])
				{
					//delete mSceneArray[i];
					mSceneArray.erase(mSceneArray.begin() + i);
					return;
				}
			}
		}
	}

	void SceneManager::DeleteScene(unsigned int index)
	{
		if (index < mSceneArray.size())
		{
			//delete mSceneArray[index];
			mSceneArray.erase(mSceneArray.begin() + index);
		}
	}

	void SceneManager::ClearAllScene()
	{
		mSceneArray.clear();
	}

	Scene* SceneManager::GetScene(unsigned int index) const
	{
		if (index < mSceneArray.size())
		{
			return mSceneArray[index];
		}
		return nullptr;
	}

	unsigned int SceneManager::GetSceneNum() const
	{
		return mSceneArray.size();
	}

}