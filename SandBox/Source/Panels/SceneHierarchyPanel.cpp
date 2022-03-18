#include "SceneHierarchyPanel.h"
#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>
#include "Engine/Graphic/World/Scene/Scene.h"
#include "Engine/Graphic/World/SceneMap.h"
#include "Engine/Graphic/World/World.h"
#include "Engine/Graphic/Object.h"
#include "Engine/Graphic/World/Actor/Actor.h"
//#include "Engine/Graphic/SkeletalMeshComponent.h"
//#include "Engine/Graphic/SkeletalMeshModel.h"
//#include "Engine/Graphic/Resource.h"
//#include "Engine/Renderer/Texture.h"
//#include "Engine/Platform/OpenGL/OpenGLTexture.h"
//#include "TextureFilePanel.h"
//#include "Engine/Graphic/ResourceManager.h"
//#include "Engine/Graphic/Property.h"
namespace FakeReal {

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, T* component, UIFunction func)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(component).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			func(component);
			ImGui::TreePop();
		}

		/*if (removeComponent)
			entity.RemoveComponent<T>();*/
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		m_pContext = nullptr;
		m_pSelectActor = nullptr;
		m_pSelectNode = nullptr;
		//m_pTextureFilePanel = eastl::make_shared<TextureFilePanel>();
	}

	SceneHierarchyPanel::~SceneHierarchyPanel()
	{
		m_pSelectActor = nullptr;
		m_pContext = nullptr;
	}

	void SceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("SceneHierarchy");
		if (m_pContext)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)m_pContext, flags, m_pContext->GetSceneName().c_str());

			if (opened)
			{
				std::vector<Actor*>& actorArray = m_pContext->GetActorArray();
				for (Actor* a : actorArray)
				{
					DrawEntity(a);
				}
				ImGui::TreePop();
			}
		}
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_pSelectActor = nullptr;

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				//m_Context->CreateEntity("Empty Entity");
				printf("Create Empty Entity\n");

			ImGui::EndPopup();
		}
		ImGui::End();

		ImGui::Begin("Components");
		DrawComponents(m_pSelectActor);
		ImGui::End();

		ImGui::Begin("Properties");
		DrawNodeProperty(m_pSelectNode);
		ImGui::End();

		//m_pTextureFilePanel->OnImguiRender();
	}

	void SceneHierarchyPanel::SetContext(SceneMap* pContext)
	{
		m_pContext = pContext;
	}

	void SceneHierarchyPanel::ClearContext()
	{
		m_pSelectActor = nullptr;
		m_pContext = nullptr;
	}

	void SceneHierarchyPanel::DrawEntity(Actor* pEntity)
	{
		if (pEntity)
		{
			ImGuiTreeNodeFlags flags = ((m_pSelectActor == pEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)pEntity, flags, pEntity->ms_Type.GetName().c_str());

			if (ImGui::IsItemClicked())
			{
				m_pSelectActor = pEntity;
			}

			bool entityDeleted = false;
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Entity"))
				{
					entityDeleted = true;
				}
				ImGui::EndPopup();
			}

			if (opened)
			{
				/*ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
				bool opened = ImGui::TreeNodeEx((void*)9817239, flags, pEntity->ms_Type.GetName().c_str());
				if (opened)
					ImGui::TreePop();*/
				ImGui::TreePop();
			}

			if (entityDeleted)
			{
				/*World::ms_pWorld->DestroyActor(pEntity);
				if (m_SelectionContext == entity)
					m_SelectionContext = {};*/
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Actor* pSelectActor)
	{
		if (pSelectActor)
		{
			//文本输入
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, pSelectActor->GetType().GetName().c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				//tag = std::string(buffer);
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(-1);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::PopItemWidth();

			//根节点
			DrawComponent<Node>("RootNode", pSelectActor->GetRootNode(), [this](Node*& pNode)
			{
				ImGui::Selectable("Selectable");
				if (ImGui::IsItemClicked())
				{
					m_pSelectNode = pNode;
				}
				//子节点
				unsigned int num = pNode->GetChildNum();
				for (unsigned int i = 0; i < num; i++)
				{
					DrawComponents((Node*)(pNode->GetChild(i)));
				}
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_pSelectNode = nullptr;

			//DrawComponent<SkeletalMeshComponent>("SkeletalMeshComponent", (SkeletalMeshComponent*)pSelectActor->GetRootComponent(), [this](SkeletalMeshComponent*& pComp)
			//{
				/*SkeletalMeshModel* pModel = pComp->GetModelResource()->GetResource();
				eastl::vector<SkeletalMesh*>& meshList = pModel->GetMeshList();

				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				ImGui::Separator();
				bool open = ImGui::TreeNodeEx((void*)typeid(&meshList).hash_code(), treeNodeFlags,"Mesh list");
				ImGui::PopStyleVar();
				if (open)
				{
					for (SkeletalMesh* p : meshList)
					{
						eastl::vector<Texture2D*>& textures = p->GetTextureList();

						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
						float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
						ImGui::Separator();
						bool open = ImGui::TreeNodeEx((void*)typeid(&textures).hash_code(), treeNodeFlags, "Textures");
						ImGui::PopStyleVar();
						if (open)
						{
							for (Texture2D* tex : textures)
							{
								const eastl::string& path = ((OpenGLTexture2D*)tex)->GetPath();
								uint64_t textureID = tex->GetRendererID();
								ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ 50.f, 50.f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
								ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
								float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
								ImGui::SameLine();
								ImGui::BeginGroup();
								ImGui::TextUnformatted(path.c_str());
								ImGui::Spacing();
								if (ImGui::Button("Select"))
								{
									m_pTextureFilePanel->OpenPanel([tex, pModel, pComp](const eastl::string& selectFile) {
										tex->SetTexture(selectFile);
										eastl::string resourceName = "Resources/Character/Models/EditorSaveModel." + Resource::GetFileSuffix(Resource::RT_SKELETON_MODEL) + ".assert";
										pModel->SetResourceName(resourceName);
										pComp->GetModelResource()->SetResourceName(resourceName);
										ResourceManager::NewSaveSkeletalMeshModel(pModel, "EditorSaveModel");
									});
								}
								ImGui::EndGroup();
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}*/
/*
				Rtti& rtti = pComp->GetType();
				eastl_size_t propertyNum = rtti.GetPropertyNum();
				for (eastl_size_t i = 0; i < propertyNum; i++)
				{
					Property* pProperty = rtti.GetProperty(i);
					if (pProperty && pProperty->GetFlag() & Property::F_REFLECT_NAME)
					{
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
						ImGui::Separator();
						ImGui::Text(pProperty->GetName().c_str());
						ImGui::PopStyleVar();
					}
				}*/
			//});
		}
	}

	void SceneHierarchyPanel::DrawComponents(Node* pComp)
	{
		DrawComponent<Node>(pComp->GetType().GetName().c_str(), pComp, [this](Node*& pNode)
		{
			//
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)pNode, flags, pNode->GetType().GetName().c_str());

			if (opened)
			{
				ImGui::TreePop();
			}
		});
	}

	void SceneHierarchyPanel::DrawNodeProperty(Node* pNode)
	{
		if (m_pSelectNode)
		{
			DrawComponent<Node>("Transform", pNode, [](Node*& component)
			{
				DrawVec3Control("Translation", component->GetWorldTranslateRef());
				/*glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);*/
			});
		}
	}

}
