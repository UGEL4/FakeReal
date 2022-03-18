#include "Actor.h"
#include "../../Node/Node.h"
#include "../../Node/NodeComponent/NodeComponent.h"
#include "../../Stream/Property.h"
#include "../SceneMap.h"

namespace FakeReal {
	IMPLEMENT_RTTI(Actor, Object);
	IMPLEMENT_INITIAL_BEGIN(Actor)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(Actor, Object)
	REGISTER_PROPERTY(m_pOwner, "Owner", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pNode, "RootNode", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mName, "Actor Name", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mChildren, "Children", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mNodeComponent, "Components", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pSceneMap, "SceneMap", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	Actor::Actor()
	{
		m_pOwner = nullptr;
		m_pNode = nullptr;
		mChildren.clear();
		mNodeComponent.clear();
		std::cout << "构造Actor:" << this << std::endl;
	}

	Actor::~Actor()
	{
		m_pOwner = nullptr;
		if (m_pNode)
			delete m_pNode;
		m_pNode = nullptr;
		for (Actor* p : mChildren)
		{
			if (p)
				delete p;
		}
		mChildren.clear();
		for (NodeComponent* p : mNodeComponent)
		{
			if (p)
				delete p;
		}
		mNodeComponent.clear();
		std::cout << "析构Actor:" << this << std::endl;
	}

	void Actor::Update(float appTime)
	{
		
	}

	void Actor::ProcessInput()
	{

	}

	void Actor::SetWorldPos(const glm::vec3& pos)
	{
		m_pNode->SetWorldTranslate(pos);
	}

	void Actor::SetWorldRotation(const glm::vec3& rotate)
	{
		m_pNode->SetWorldRotation(rotate);
	}

	void Actor::SetWorldRotation(const glm::quat& rotate)
	{
		m_pNode->SetWorldRotation(rotate);
	}

	void Actor::SetWorldScale(const glm::vec3& scale)
	{
		m_pNode->SetWorldScale(scale);
	}

	void Actor::SetLocalPos(const glm::vec3& pos)
	{
		m_pNode->SetLocalTranslate(pos);
	}

	void Actor::SetLocalRotation(const glm::vec3& rotate)
	{
		m_pNode->SetLocalRotation(rotate);
	}

	void Actor::SetLocalRotation(const glm::quat& rotate)
	{
		m_pNode->SetLocalRotation(rotate);
	}

	void Actor::SetLocalScale(const glm::vec3& scale)
	{
		m_pNode->SetLocalScale(scale);
	}

	glm::mat4 Actor::GetWorldMatrix() const
	{
		return m_pNode->GetWorldMatrix();
	}

	const Transform& Actor::GetWorldTransform()
	{
		return m_pNode->GetWorldTransform();
	}

	void Actor::DeleteComponentNode(NodeComponent* pComponent)
	{
		if (pComponent == m_pNode)
			return;
		
		std::vector<NodeComponent*>::iterator itr = std::find(mNodeComponent.begin(), mNodeComponent.end(), pComponent);
		if (itr == mNodeComponent.end())
			return;

		mNodeComponent.erase(itr);
		
		std::vector<Spatial*> Children = pComponent->GetChildList();
		for (auto p : Children)
		{
			if (p)
			{
				Node* pNode = DynamicCast<Node>(pComponent->GetParent());
				pNode->AddChild(p);
			}
		}
		pComponent->RemoveAllChild();
		((Node*)(pComponent->GetParent()))->RemoveChild(pComponent);
		delete pComponent;
	}

	void Actor::ChangeComponentNodeParent(NodeComponent* pComponent, Node* pParent /*= nullptr*/)
	{
		if (pComponent == m_pNode || pComponent->GetParent() == pParent)
		{
			return;
		}
		std::vector<NodeComponent*>::iterator itr = std::find(mNodeComponent.begin(), mNodeComponent.end(), pComponent);
		if (itr == mNodeComponent.end())
		{
			return;
		}

		if (!pParent)
		{
			pParent = m_pNode;
		}

		if (pParent != m_pNode)
		{
			NodeComponent* pParentNode = DynamicCast<NodeComponent>(pParent);
			if (pParentNode)
			{
				std::vector<NodeComponent*>::iterator itr = std::find(mNodeComponent.begin(), mNodeComponent.end(), pParentNode);
				if (itr == mNodeComponent.end())
				{
					return;
				}
			}
		}

		Node* pParentNode = DynamicCast<Node>(pComponent->GetParent());
		pParentNode->RemoveChild(pComponent);
		pParent->AddChild(pComponent);
	}

	void Actor::CreateDefaultComponent()
	{
		m_pNode = NodeComponent::CreateComponent<NodeComponent>();
	}

	void Actor::AddChildActor(Actor* pActor)
	{
		if (pActor)
		{
			size_t i = 0;
			for (; i < mChildren.size(); i++)
			{
				if (pActor == mChildren[i])
				{
					break;
				}
			}
			if (i == mChildren.size())
			{
				//这里会把pActor的根节点添加到scene中
				m_pSceneMap->AddActor(pActor);
				mChildren.emplace_back(pActor);
			}
			Node* pNode = DynamicCast<Node>(m_pNode);
			//这里把pActor的根节点从scene中移除，因为pActor的根节点会挂到this的根节点下
			m_pSceneMap->GetScene()->RemoveComponent(pActor->GetRootNode());
			//pActor的根节点挂到this的根节点下
			pNode->AddChild(pActor->GetRootNode());
			pActor->m_pOwner = this;
		}
	}

	void Actor::RemoveChildActor(Actor* pActor)
	{
		for (size_t i = 0; i < mChildren.size(); i++)
		{
			if (mChildren[i] == pActor)
			{

				Node* pNode = DynamicCast<Node>(pActor->GetRootNode()->GetParent());
				pNode->RemoveChild(pActor->GetRootNode());
				pActor->m_pOwner = NULL;
				mChildren.erase(mChildren.begin() + i);
				return;
			}
		}
	}

	void Actor::DeleteChildActor(Actor* pActor)
	{
		for (size_t i = 0; i < mChildren.size(); i++)
		{
			if (mChildren[i] == pActor)
			{

				Node* pNode = DynamicCast<Node>(pActor->GetRootNode()->GetParent());
				pNode->RemoveChild(pActor->GetRootNode());
				pActor->m_pOwner = NULL;
				mChildren.erase(mChildren.begin() + i);
				delete pActor;
				return;
			}
		}
	}

	void Actor::AddToSceneMap(SceneMap* pSceneMap)
	{
		m_pSceneMap = pSceneMap;
	}

	void Actor::OnDestroy()
	{
		if (m_pSceneMap)
			m_pSceneMap->RemoveActor(this);
	}

}