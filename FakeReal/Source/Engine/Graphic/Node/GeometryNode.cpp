#include "GeometryNode.h"
#include "../Stream/Property.h"
namespace FakeReal {
	IMPLEMENT_RTTI(GeometryNode, Node);
	IMPLEMENT_INITIAL_BEGIN(GeometryNode)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(GeometryNode, Node)
	END_ADD_PROPERTY

	GeometryNode::GeometryNode()
	{

	}
	GeometryNode::~GeometryNode()
	{
		std::cout << "Îö¹¹GeometryNode:" << this << std::endl;
	}

	Geometry* GeometryNode::GetGeometry(unsigned int uiIndex) const
	{
		if (uiIndex >= m_pChildList.size())
			return nullptr;
		else
		{
			Geometry* p = (Geometry*)m_pChildList[uiIndex];
			return p;
		}
	}

}