#pragma once
#include "MeshNodeComponent.h"
#include "../Model/StaticMeshNode.h"

namespace FakeReal {
	class FR_ENGINE_API StaticMeshNodeComponent : public MeshNodeComponent
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		StaticMeshNodeComponent();
		~StaticMeshNodeComponent();

		StaticMeshNode* GetStaticMeshNode();
		void SetStaticMeshResource(StaticMeshNodeRPtr pResource);

		virtual void PostCreate() override;
		virtual void LoadedEvent(ResourceProxyBase* pResourceProxy, int data = 0) override;

	private:
		StaticMeshNodeRPtr m_pRMeshNode;
		bool mbUseDefaultMesh;
	};
	FR_TYPE_MARCO(StaticMeshNodeComponent)

}