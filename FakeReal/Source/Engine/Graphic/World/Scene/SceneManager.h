#pragma once
#include "../../../Core/CoreMarco.h"
#include "Scene.h"
/*
	World���𴴽�ʵ��͵�ͼ��SceneMap��������ʵ�塢��ʵ�巢��Ϣ��SceneMap����Լ�ӵ�е�ʵ�壬��ӵ��һ��Sceneָ�룬Scene���ǹ���Actor�������
	���磨��ͼ��ʵ�壨���������<=== = > ����������������������ʵ����������
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