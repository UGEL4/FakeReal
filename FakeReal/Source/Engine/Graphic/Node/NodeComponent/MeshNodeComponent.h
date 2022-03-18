#pragma once
#include "NodeComponent.h"
#include "../Model/ModelMeshNode.h"

namespace FakeReal {
	class FR_ENGINE_API MeshNodeComponent : public NodeComponent
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		MeshNodeComponent();
		virtual ~MeshNodeComponent();

		virtual void UpdateNodeAll(float appTime) override;
		virtual bool IsNeedDraw() const override
		{
			return true;
		}

	protected:
		ModelMeshNode* m_pMeshNode;
	};
	FR_TYPE_MARCO(MeshNodeComponent)
}