#include "NodeComponent.h"
#include "../../Stream/Property.h"

namespace FakeReal {
	IMPLEMENT_RTTI(NodeComponent, Node);
	IMPLEMENT_INITIAL_BEGIN(NodeComponent)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(NodeComponent, Node)
	END_ADD_PROPERTY

	NodeComponent::NodeComponent()
	{

	}

	NodeComponent::~NodeComponent()
	{
		std::cout << "Îö¹¹NodeComponent:" << this << std::endl;
	}

	void NodeComponent::UpdateNodeAll(float appTime)
	{
		UpdateTransform(appTime);
		for (auto& p : m_pChildList)
		{
			if (p)
				p->UpdateNodeAll(appTime);
		}
		mbIsChange = false;
	}

}
