#include "StaticActor.h"
#include "../../Stream/Property.h"
#include "../../Node/NodeComponent/StaticMeshNodeComponent.h"

namespace FakeReal {
	IMPLEMENT_RTTI(StaticActor, Actor);
	IMPLEMENT_INITIAL_BEGIN(StaticActor)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(StaticActor, Actor)
	END_ADD_PROPERTY

	StaticActor::StaticActor()
	{
		std::cout << "¹¹ÔìStaticActor:" << this << std::endl;
	}

	StaticActor::~StaticActor()
	{
		std::cout << "Îö¹¹StaticActor:" << this << std::endl;
	}

	void StaticActor::CreateDefaultComponent()
	{
		m_pNode = NodeComponent::CreateComponent<StaticMeshNodeComponent>();
	}

	void StaticActor::SetStaticMeshResource(StaticMeshNodeRPtr pResource)
	{
		if (!pResource)
			return;

		StaticMeshNodeComponent* pComp = (StaticMeshNodeComponent*)m_pNode;
		if (pComp)
		{
			pComp->SetStaticMeshResource(pResource);
		}
	}

}