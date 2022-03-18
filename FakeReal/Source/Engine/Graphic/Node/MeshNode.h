#pragma once
/************************************************************************/
/* 只维护一些基本状态，例如是否接受投影、是否投射阴影、是否接受光照等   */
/************************************************************************/
#include "Node.h"
#include "../Resource/Resource.h"

namespace FakeReal {
	class FR_ENGINE_API MeshNode : public Node, public Resource
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		MeshNode();
		virtual ~MeshNode();

		virtual unsigned int GetResourceType() const override { return RT_MAX; }
	};
	FR_TYPE_MARCO(MeshNode);
	DECLARE_Proxy(MeshNode)
}