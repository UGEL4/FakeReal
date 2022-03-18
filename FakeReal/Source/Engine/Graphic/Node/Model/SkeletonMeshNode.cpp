#include "SkeletonMeshNode.h"
#include "../GeometryNode.h"
#include "../../Stream/Property.h"

namespace FakeReal {
	SkeletonMeshNode* SkeletonMeshNode::m_pDefault = nullptr;
	IMPLEMENT_RTTI(SkeletonMeshNode, ModelMeshNode);
	IMPLEMENT_INITIAL_BEGIN(SkeletonMeshNode)
	ADD_PRIORITY(Geometry)
	ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
	ADD_TERMINATE_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(SkeletonMeshNode, ModelMeshNode)
	REGISTER_PROPERTY(m_pSkeleton, "Skeleton", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	SkeletonMeshNode::SkeletonMeshNode()
	{

	}

	SkeletonMeshNode::~SkeletonMeshNode()
	{
		std::cout << "析构SkeletonMeshNode:" << this << std::endl;
	}

	bool SkeletonMeshNode::InitialDefaultState()
	{
		m_pDefault = new SkeletonMeshNode();
		GeometryNode* pGeometryNode = new GeometryNode();
		m_pDefault->AddChild(pGeometryNode);
		//TODO:这里要克隆
		Geometry* pGeometry = (Geometry*)Object::CloneCreateObject(Geometry::GetDefaultRenderCube());
		//Geometry* pGeometry = Geometry::GetDefaultRenderCube();
		pGeometryNode->AddChild(pGeometry);
		//m_pDefault->CreateLocalAABB();
		pGeometryNode->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		return true;
	}

	bool SkeletonMeshNode::TerminalDefaultState()
	{
		delete m_pDefault;
		m_pDefault = nullptr;
		return true;
	}
}