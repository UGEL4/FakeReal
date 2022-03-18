#pragma once
#include "../../Object.h"
#include "../../Node/Node.h"
#include "../../Node/NodeComponent/NodeComponent.h"
#include <vector>
#include <glm/glm.hpp>

namespace FakeReal
{
	class NodeComponent;
	class Transform;
	class SceneMap;
	class FR_ENGINE_API Actor : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		Actor();
		virtual ~Actor();

		virtual void Update(float appTime);
		virtual void ProcessInput();


		//getter and setter
		virtual void SetWorldPos(const glm::vec3& pos);
		virtual void SetWorldRotation(const glm::vec3& rotate);
		virtual void SetWorldRotation(const glm::quat& rotate);
		virtual void SetWorldScale(const glm::vec3& scale);

		virtual void SetLocalPos(const glm::vec3& pos);
		virtual void SetLocalRotation(const glm::vec3& rotate);
		virtual void SetLocalRotation(const glm::quat& rotate);
		virtual void SetLocalScale(const glm::vec3& scale);

		Actor* GetOwner() const { return m_pOwner; }
		glm::mat4 GetWorldMatrix() const;
		Node* GetRootNode() const { return m_pNode; }
		const Transform& GetWorldTransform();
		void SetName(const std::string& name) { mName = name; }
		std::string& GetName() { return mName; }

		//component
		template<class T>
		T* AddComponentNode();
		template<class T>
		void GetComponentNode(std::vector<T*>& list);
		void DeleteComponentNode(NodeComponent* pComponent);
		void ChangeComponentNodeParent(NodeComponent* pComponent, Node* pParent = nullptr);
		virtual void CreateDefaultComponent();

		//Actor
		virtual void AddChildActor(Actor* pActor);
		//只移除，不释放pActor
		virtual void RemoveChildActor(Actor* pActor);
		//删除pActor并释放内存
		virtual void DeleteChildActor(Actor* pActor);
		size_t GetChildrenNum() const { return mChildren.size(); }

		//scene
		void AddToSceneMap(SceneMap* pSceneMap);
		SceneMap* GetSceneMap() const { return m_pSceneMap; }

		void OnDestroy();
	protected:
		Actor* m_pOwner;
		Node* m_pNode;
		std::string mName;
		std::vector<Actor*> mChildren;
		std::vector<NodeComponent*> mNodeComponent;//不包含根节点
		SceneMap* m_pSceneMap;
	};
	FR_TYPE_MARCO(Actor)

	template<class T>
	void Actor::GetComponentNode(std::vector<T*>& list)
	{
		for (auto& comp : mNodeComponent)
		{
			if (comp->GetType().IsSameType(T::GetType()))
			{
				list.emplace_back(comp);
			}
		}
	}

	template<class T>
	T* Actor::AddComponentNode()
	{
		T* Comp = NodeComponent::CreateComponent<T>();
		if (Comp)
		{
			m_pNode->AddChild(Comp);
			mNodeComponent.emplace_back(Comp);
			return Comp;
		}
		else
			return nullptr;
	}

}