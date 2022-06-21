#include "Editor.h"
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Shader/Shader.h"
#include "Engine/Graphic/Node/GeometryNode.h"
#include "Engine/Graphic/Node/Mesh/Mesh.h"
#include "Engine/Graphic/Node/Model/StaticMeshNode.h"
#include "Engine/Graphic/Stream/Stream.h"
#include "Engine/Graphic/World/Actor/EditorCameraActor.h"
#include "Engine/Render/RenderResource/ResourceBind.h"
#include "Engine/Render/RenderResource/RenderResource.h"
#include "Engine/Graphic/Resource/Resource.h"
#include "Engine/Render/Texture/FRTexture.h"
#include "Engine/Graphic/World/World.h"
#include "Engine/Graphic/World/SceneMap.h"
#include "Engine/Core/Application.h"
#include "Panels/SceneHierarchyPanel.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <imgui.h>

namespace FakeReal {

	GameEditor::GameEditor()
	{
		m_pSceneHierarchyPanel = new SceneHierarchyPanel;
	}

	GameEditor::~GameEditor()
	{

	}

	void GameEditor::OnAttach()
	{
	}

	void GameEditor::OnDetach()
	{
		/*std::string& fileSuffix = Resource::GetFileSuffix(Resource::RT_TEXTURE);
		Stream saveStream;
		saveStream.SetStreamFlag(Stream::AT_REGISTER);
		saveStream.ArchiveAll(m_pTexture);
		saveStream.NewSave(("Resources/models/Textures/Stormtrooper_D.png." + fileSuffix + ".assert").c_str());*/
		
		//delete m_pStaticActor;
		delete m_pCameraActor;
		if (m_pSkyBox)
			delete m_pSkyBox;

		if (m_pSceneHierarchyPanel)
			delete m_pSceneHierarchyPanel;
	}

	void GameEditor::Start()
	{
		m_pFrameBuffer = FrameBuffer::Create(1280, 720);

		m_pCameraActor = new EditorCameraActor;
		m_pCameraActor->CreateDefaultComponent();

		//test world
		m_pCurrMap = World::ms_pWorld->GetSceneMap(0);//默认地图
		m_pStaticActor = (StaticActor*)World::ms_pWorld->CreateActor<StaticActor>({ 0.0f, 0.0f, -3.0f });
		m_pSceneHierarchyPanel->SetContext(m_pCurrMap);

		/*m_pStaticActor = new StaticActor;
		m_pStaticActor->CreateDefaultComponent();
		m_pStaticActor->SetWorldPos({ 0.0f, 0.0f, -3.0f });
		m_pStaticActor->SetWorldRotation({ 0.f, 0.f, 0.f });
		//m_pStaticActor->SetWorldScale({ 0.02f, 0.02f, 0.02f });
		m_pStaticActor->SetWorldScale({ 1.f, 1.f, 1.f });*/

		m_pShader = ShaderLibrary::gs_pShaderLib->Get("BaseShader");

		/*m_pComponent = new StaticMeshNodeComponent;
		m_pComponent->SetWorldTranslate({ 0.0f, 0.0f, -5.0f });
		m_pComponent->SetWorldRotation({ 45.0f, 45.0f, 0.0f });
		m_pComponent->UpdateTransform(0.0f);*/


		//测试模型
		Stream loadStream;
		loadStream.NewLoad("Resources/models/silly_dancing.fbx.static_model.assert");
		//loadStream.NewLoad("Resources/models/Sponza.fbx.static_model.assert");
		//loadStream.NewLoad("Resources/models/3d_scene.static_model.assert");
		//loadStream.NewLoad("Resources/models/Apocalypse.fbx.static_model.assert");
		StaticMeshNode* pMeshNode = (StaticMeshNode*)loadStream.GetObjectByRtti(StaticMeshNode::ms_Type);

		StaticMeshNodeRPtr StaticMeshResource = ResourceProxy<StaticMeshNode>::Create(pMeshNode);
		m_pStaticActor->SetStaticMeshResource(StaticMeshResource);
		m_pStaticActor->SetWorldScale({ 0.05f, 0.05f, 0.05f });

		/*LoadImageData data = Texture2D::LoadImage("Resources/pic.jpeg", true);
		if (data.data)
		{
			Texture* pTexture = new Texture2D("Resources/pic.jpeg", data.width, data.height, data.format);
			pTexture->SetData(data.data, data.dataSize);
			SamplerDesc desc;
			SamplerState* pSamplerState = new SamplerState;
			pSamplerState->SetSamplerDesc(desc);
			m_pTexture = new TextureAllState(pTexture);
			m_pTexture->SetSamplerState(pSamplerState);
			TextureID* id = Renderer::CreateTextureID(pTexture, pSamplerState);
			m_pTexture->SetTextureID(id);
		}*/

		//测试纹理
		Stream loadTexturesStream;
		loadTexturesStream.NewLoad("Resources/models/Textures/Stormtrooper_D.png.texture.assert");
		m_pTexture = (TextureAllState*)loadTexturesStream.GetObjectByRtti(TextureAllState::ms_Type);

		//天空盒
		m_pSkyBox = new SkyBox;
		m_pSkyBox->InitRenderResource();

		//骨骼模型
		m_pSkMeshComp = new SkeletonMeshComponent;
		Stream skloadStream;
		skloadStream.NewLoad("Resources/models/silly_dancing.fbx.skeletal_model.assert");
		SkeletonMeshNode* pSkMeshNode = (SkeletonMeshNode*)skloadStream.GetObjectByRtti(SkeletonMeshNode::ms_Type);
		SkeletonMeshNodeRPtr pSkMeshNodeR = SkeletonMeshNodeR::Create(pSkMeshNode);
		m_pSkMeshComp->SetSkeletonMeshResource(pSkMeshNodeR);
		m_pSkMeshComp->SetWorldTranslate({ 5.0f, 0.0f, -3.0f });
		m_pSkMeshComp->UpdateAll(0.0f);
		m_pSkMeshComp->SetAnimation();
		m_pSkMeshComp->PlayAnimation("dance");
		ShaderLibrary::gs_pShaderLib->LoadShader("Shader/OpenGL/SkeletalMesh.glshader");

		//test save dds
		Texture2D::LoadImageCompressAndSaveDDS("Resources/Stormtrooper_D.png", "Resources/models/Textures/Stormtrooper_D.dds", FRNVTTCompression::CF_BC3);
	}

	void GameEditor::OnUpdate(float ts)
	{
		// Resize
		if (mViewportSize.x > 0.0f && mViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(m_pFrameBuffer->GetViewportWidth() != mViewportSize.x || m_pFrameBuffer->GetViewportHeight() != mViewportSize.y))
		{
			m_pFrameBuffer->Resize((unsigned int)mViewportSize.x, (unsigned int)mViewportSize.y);
			m_pCameraActor->SetViewPortSize(mViewportSize.x, mViewportSize.y);
		}

		//update
		m_pCameraActor->ProcessInput();
		m_pCameraActor->Update(ts);
		m_pSkMeshComp->UpdateAll(ts);
		//m_pStaticActor->GetRootNode()->UpdateAll(ts);

		//render
		m_pFrameBuffer->Bind();
		Renderer::SetClearColor(0.f, 1.f, 0.f, 1.f);
		Renderer::Clear();
		Renderer::BeginScene(m_pCameraActor->GetViewProjMatrix());
		//Geometry* pGeo = m_pComponent->GetStaticMeshNode()->GetDefault()->GetGeometryNode(0)->GetGeometry(0);
		StaticMeshNodeComponent* pMeshNodeCompo = (StaticMeshNodeComponent*)m_pStaticActor->GetRootNode();
		//Geometry* pGeo = pMeshNodeCompo->GetStaticMeshNode()->GetGeometryNode(0)->GetGeometry(0);
		unsigned int GeoNum = pMeshNodeCompo->GetStaticMeshNode()->GetGeometryNode(0)->GetChildNum();
		m_pShader->Bind();
		m_pShader->SetInt("m_Material.texture_diffuse", 1);
		//m_pShader->SetMatrix4("worldMat", &m_pComponent->GetWorldMatrix()[0][0]);
		m_pShader->SetMatrix4("worldMat", &m_pStaticActor->GetWorldMatrix()[0][0]);
		//pGeo->GetMeshData()->GetTextureList()[0]->GetResource()->BindTexture(1);
		for (unsigned int i = 0; i < GeoNum; i++)
		{
			m_pTexture->BindTexture(1);
			Geometry* pGeo = pMeshNodeCompo->GetStaticMeshNode()->GetGeometryNode(0)->GetGeometry(i);
			Renderer::Submit(*m_pShader, *(pGeo->GetVertexArray()), pGeo->GetIndexCount());
		}
		//m_pShader->Unbind();

		//Shader* pShaderSkeletal = ShaderLibrary::gs_pShaderLib->Get("SkeletalMesh");
		//pShaderSkeletal->Bind();
		//pShaderSkeletal->SetInt("m_Material.texture_diffuse", 0);
		//pShaderSkeletal->SetMatrix4("worldMat", &m_pSkMeshComp->GetWorldMatrix()[0][0]);
		//GeoNum = m_pSkMeshComp->GetSkeletonMeshNode()->GetGeometryNode(0)->GetChildNum();
		//for (unsigned int i = 0; i < GeoNum; i++)
		//{
		//	m_pTexture->BindTexture(0);
		//	Geometry* pGeo = m_pSkMeshComp->GetSkeletonMeshNode()->GetGeometryNode(0)->GetGeometry(i);
		//	m_pSkMeshComp->ComputeMatrixPalette();
		//	m_pSkMeshComp->TestPlayAnim(pShaderSkeletal);
		//	Renderer::Submit(*pShaderSkeletal, *(pGeo->GetVertexArray()), pGeo->GetIndexCount());
		//}

		//天空盒子
		Shader* pShaderSky = ShaderLibrary::gs_pShaderLib->Get("SkyBox");
		m_pSkyBox->BeforeDraw(pShaderSky, m_pCameraActor);
		Renderer::SetDepthFunc(Renderer::DF_LESS_EQUAL);
		Geometry* pGeo = (Geometry*)m_pSkyBox->GetMeshNode()->GetChild(0);
		Renderer::Submit(*pShaderSky, *(pGeo->GetVertexArray()), pGeo->GetIndexCount());
		Renderer::SetDepthFunc(Renderer::DF_LESS);
		Renderer::EndScene();
		m_pFrameBuffer->Unbind();
	}

	void GameEditor::OnImGuiRender()
	{
		//This code copy from imgui_demo.cpp
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();
		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		//DockSpace begin
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					/*else
					{
						CreateScene();
					}*/
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{

				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{

				}

				if (ImGui::MenuItem("Exit"))
				{
					FakeReal::Application::GetInstance()->OnClose();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//Panels
		m_pSceneHierarchyPanel->OnImguiRender();
		//m_pContentBrowserPanel->OnImguiRender();

		//static bool iopen = true;
		//ShowExampleAppLayout(&iopen);
		//ShowExampleAppSimpleOverlay(&open);悬浮
		//ShowExampleAppCustomRendering(&iopen);
		//ShowExampleAppFullscreen(&iopen);
		//ShowExampleAppMainMenuBar();
		//ShowExampleAppPropertyEditor(&iopen);
		//ShowExampleAppAutoResize(&open);
		//ShowExampleAppConstrainedResize(&open);
		//ShowExampleMenuFile();
		//ShowExampleAppConsole(&open);
		//ShowExampleAppWindowTitles(&open);

		//viewport begin
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		//m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		//m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
		//m_ViewportFocused = ImGui::IsWindowFocused();
		//m_ViewportHovered = ImGui::IsWindowHovered();
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint64_t textureID = m_pFrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			/*if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}*/
			ImGui::EndDragDropTarget();
		}
		ImGui::End();
		ImGui::PopStyleVar();
		//viewport end

		//UI_Toolbar();

		ImGui::End();
	}

	void GameEditor::OnEvent(Event& event)
	{
		m_pCameraActor->OnEvent(event);
	}

	void GameEditor::UI_Toolbar()
	{
		
	}

}
