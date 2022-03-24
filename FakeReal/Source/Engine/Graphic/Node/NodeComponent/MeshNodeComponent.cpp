#include "MeshNodeComponent.h"
#include "../../Stream/Property.h"

namespace FakeReal {
	IMPLEMENT_RTTI(MeshNodeComponent, NodeComponent);
	IMPLEMENT_INITIAL_BEGIN(MeshNodeComponent)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(MeshNodeComponent, NodeComponent)
	REGISTER_PROPERTY(m_pMeshNode, "Mesh Node", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	MeshNodeComponent::MeshNodeComponent()
	{
		m_pMeshNode = nullptr;
	}
	MeshNodeComponent::~MeshNodeComponent()
	{
		delete m_pMeshNode;
		std::cout << "Îö¹¹MeshNodeComponent:" << this << std::endl;
	}

	void MeshNodeComponent::UpdateNodeAll(float appTime)
	{
		if (appTime > 0.0f)
		{
			UpdateController(appTime);
		}
		UpdateTransform(appTime);
		if (m_pMeshNode)
			m_pMeshNode->UpdateAll(appTime);
		mbIsChange = false;
	}

}