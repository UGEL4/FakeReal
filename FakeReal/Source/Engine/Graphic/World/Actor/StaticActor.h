#pragma once

#include "Actor.h"
#include "../../Node/Model/StaticMeshNode.h"

namespace FakeReal {
	class FR_ENGINE_API StaticActor : public Actor
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		StaticActor();
		virtual ~StaticActor();

		virtual void CreateDefaultComponent() override;

		void SetStaticMeshResource(StaticMeshNodeRPtr pResource);
	};
	FR_TYPE_MARCO(StaticActor)
}