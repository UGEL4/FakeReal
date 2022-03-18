#include "MeshNode.h"
#include "../Stream/Property.h"
namespace FakeReal {
	IMPLEMENT_RTTI(MeshNode, Node);
	IMPLEMENT_INITIAL_BEGIN(MeshNode)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(MeshNode, Node)
	END_ADD_PROPERTY

	MeshNode::MeshNode()
	{
	}

	MeshNode::~MeshNode()
	{
		std::cout << "Îö¹¹MeshNode:" << this << std::endl;
	}

}