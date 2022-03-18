#pragma once

#include "Actor.h"

namespace FakeReal
{
	class FR_ENGINE_API CameraActor : public Actor
	{
		DECLARE_RTTI
		DECLARE_INITIAL

	public:
		CameraActor();
		virtual ~CameraActor();

		virtual void Update(float appTime) override;
		virtual void CreateDefaultComponent() override;
	};
	FR_TYPE_MARCO(CameraActor)
}