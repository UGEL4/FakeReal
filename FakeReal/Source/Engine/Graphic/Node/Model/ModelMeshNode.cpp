#include "ModelMeshNode.h"
#include "../../Stream/Property.h"

namespace FakeReal {
	IMPLEMENT_RTTI(ModelMeshNode, MeshNode);
	IMPLEMENT_INITIAL_BEGIN(ModelMeshNode)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(ModelMeshNode, MeshNode)
	END_ADD_PROPERTY

	ModelMeshNode::ModelMeshNode()
	{

	}

	ModelMeshNode::~ModelMeshNode()
	{
		std::cout << "Îö¹¹ModelMeshNode:" << this << std::endl;
	}

	GeometryNode* ModelMeshNode::GetGeometryNode(unsigned int uiLodLevel) const
	{
		if (m_pChildList.size() > 1)
			return nullptr;
		else
		{
			GeometryNode* p = (GeometryNode*)m_pChildList[0];
			return p; 
		}
	}

}