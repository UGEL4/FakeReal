#pragma once
//#include <Engine/Renderer/Texture.h>
namespace FakeReal {
	class SceneMap;
	class Object;
	class Actor;
	class Node;
	//class TextureFilePanel;
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		~SceneHierarchyPanel();

		void OnImguiRender();

		void SetContext(SceneMap* pContext);
		void ClearContext();

		void DrawEntity(Actor* pEntity);
		void DrawComponents(Actor* pSelectActor);
		void DrawComponents(Node* pComp);
		void DrawNodeProperty(Node* pNode);

		SceneMap* m_pContext;
		Actor* m_pSelectActor;
		Node* m_pSelectNode;

		//texture file panel
		//eastl::shared_ptr<TextureFilePanel> m_pTextureFilePanel;
	};
}
