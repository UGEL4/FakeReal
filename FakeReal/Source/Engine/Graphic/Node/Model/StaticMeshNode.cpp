#include "StaticMeshNode.h"
#include "../GeometryNode.h"
#include "../../Stream/Property.h"

namespace FakeReal {
	StaticMeshNode* StaticMeshNode::m_pDefault = nullptr;
	IMPLEMENT_RTTI(StaticMeshNode, ModelMeshNode);
	IMPLEMENT_INITIAL_BEGIN(StaticMeshNode)
	ADD_PRIORITY(Geometry)
	ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
	ADD_TERMINATE_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(StaticMeshNode, ModelMeshNode)
	END_ADD_PROPERTY

	StaticMeshNode::StaticMeshNode()
	{

	}

	StaticMeshNode::~StaticMeshNode()
	{
		std::cout << "析构StaticMeshNode:" << this << std::endl;
	}

	bool StaticMeshNode::InitialDefaultState()
	{
		m_pDefault = new StaticMeshNode();
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

	bool StaticMeshNode::TerminalDefaultState()
	{
		delete m_pDefault;
		m_pDefault = nullptr;
		return true;
	}
}