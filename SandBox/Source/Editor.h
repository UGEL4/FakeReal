#pragma once

#include "Engine/Core/Layer/Layer.h"
#include "Engine/Render/Buffer/FrameBuffer.h"
#include "Engine/Graphic/Node/NodeComponent/StaticMeshNodeComponent.h"
#include "Engine/Graphic/Node/NodeComponent/SkeletonMeshComponent.h"
#include "Engine/Graphic/World/Actor/StaticActor.h"
#include "Engine/Graphic/Node/NodeComponent/Camera/Camera.h"
#include "Engine/Render/Texture/TextureAllState.h"
#include "Engine/Graphic/World/Actor/SkyBox.h"
#include <glm/glm.hpp>
namespace FakeReal
{
	class Shader;
	class EditorCameraActor;
	class SceneMap;
	class SceneHierarchyPanel;
	class GameEditor : public Layer
	{
	public:
		enum class SceneState
		{
			None,
			Edit,
			Play
		};
	public:
		GameEditor();
		~GameEditor();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void Start() override;
		virtual void OnUpdate(float ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void UI_Toolbar();

	private:
		FrameBuffer* m_pFrameBuffer;
		SceneState mSceneState;
		glm::vec2 mViewportSize = { 0.0f, 0.0f };
		Shader* m_pShader;
		EditorCameraActor* m_pCameraActor;
		StaticActor* m_pStaticActor;
		TextureAllState* m_pTexture;
		//StaticMeshNodeComponent* m_pComponent;
		SkyBox* m_pSkyBox;
		SceneMap* m_pCurrMap;
		SkeletonMeshComponent* m_pSkMeshComp;

		SceneHierarchyPanel* m_pSceneHierarchyPanel;

	};
}
