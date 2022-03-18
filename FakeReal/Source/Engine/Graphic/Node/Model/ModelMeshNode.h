#pragma once
#include "../MeshNode.h"

namespace FakeReal {
	class GeometryNode;
	class FR_ENGINE_API ModelMeshNode : public MeshNode
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		ModelMeshNode();
		virtual ~ModelMeshNode();

		virtual unsigned int GetResourceType() const override { return RT_MAX; }

		GeometryNode* GetGeometryNode(unsigned int uiLodLevel) const;

	};
	FR_TYPE_MARCO(ModelMeshNode)
}