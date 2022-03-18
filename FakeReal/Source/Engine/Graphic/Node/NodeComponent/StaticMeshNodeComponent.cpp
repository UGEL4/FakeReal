#include "StaticMeshNodeComponent.h"
#include "../../Resource/ResourceManager.h"
#include "../../Stream/Property.h"
namespace FakeReal {
	IMPLEMENT_RTTI(StaticMeshNodeComponent, MeshNodeComponent);
	IMPLEMENT_INITIAL_BEGIN(StaticMeshNodeComponent)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(StaticMeshNodeComponent, MeshNodeComponent)
	REGISTER_PROPERTY(m_pRMeshNode, "StaticMesh Node Resource", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	StaticMeshNodeComponent::StaticMeshNodeComponent()
	{
		m_pRMeshNode = ResourceManager::ms_pDefaultStaticMeshNodeResource;
		mbUseDefaultMesh = true;
	}

	StaticMeshNodeComponent::~StaticMeshNodeComponent()
	{
		if (!mbUseDefaultMesh)
			delete m_pRMeshNode;
		std::cout << "Îö¹¹StaticMeshNodeComponent:" << this << std::endl;
	}

	StaticMeshNode* StaticMeshNodeComponent::GetStaticMeshNode()
	{
		if (!m_pRMeshNode)
			return nullptr;
		
		return m_pRMeshNode->GetResource();
	}

	void StaticMeshNodeComponent::SetStaticMeshResource(StaticMeshNodeRPtr pResource)
	{
		if (pResource == m_pRMeshNode)
			return;
		m_pRMeshNode = pResource;
		mbUseDefaultMesh = false;
		PostCreate();
	}

	void StaticMeshNodeComponent::PostCreate()
	{
		if (!m_pRMeshNode)
			return;
		LoadedEvent(nullptr);
		if (!m_pRMeshNode->IsLoaded())
		{
			m_pRMeshNode->AddLoadEventObject(this);
		}
	}

	void StaticMeshNodeComponent::LoadedEvent(ResourceProxyBase* pResourceProxy, int data)
	{
		if (m_pRMeshNode)
		{
			if (m_pMeshNode)
			{
				m_pMeshNode->SetParent(nullptr);
				delete m_pMeshNode;
			}
			m_pMeshNode = (ModelMeshNode*)Object::CloneCreateObject(m_pRMeshNode->GetResource());
			//m_pMeshNode = m_pRMeshNode->GetResource();
			m_pMeshNode->SetParent(this);
			mbStatic = !m_pMeshNode->IsDynamic();
			mbUseDefaultMesh = false;
		}
	}

}