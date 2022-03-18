#pragma once
/************************************************************************/
/* 管理多个Geometry                                                     */
/************************************************************************/
#include "Node.h"
#include "Geometry.h"

namespace FakeReal {
	class FR_ENGINE_API GeometryNode : public Node
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		GeometryNode();
		~GeometryNode();

		Geometry* GetGeometry(unsigned int uiIndex) const;
	};
	FR_TYPE_MARCO(GeometryNode)
}