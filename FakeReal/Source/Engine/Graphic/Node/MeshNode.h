#pragma once
/************************************************************************/
/* ֻά��һЩ����״̬�������Ƿ����ͶӰ���Ƿ�Ͷ����Ӱ���Ƿ���ܹ��յ�   */
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